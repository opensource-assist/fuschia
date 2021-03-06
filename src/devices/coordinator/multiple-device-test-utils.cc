// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <lib/fidl/transformer.h>
#include <zircon/fidl.h>
#include <zircon/types.h>

#include "multiple-device-test.h"

// Reads a CreateDevice from remote, checks expectations, and sends a ZX_OK
// response.
void MultipleDeviceTestCase::CheckCreateDeviceReceived(const zx::channel& remote,
                                                       const char* expected_driver,
                                                       zx::channel* device_coordinator_remote,
                                                       zx::channel* device_controller_remote) {
  // Read the CreateDevice request.
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  zx_status_t status = remote.read(0, bytes, handles, sizeof(bytes), fbl::count_of(handles),
                                   &actual_bytes, &actual_handles);
  ASSERT_OK(status);
  ASSERT_LT(0, actual_bytes);
  ASSERT_EQ(4, actual_handles);
  *device_coordinator_remote = zx::channel(handles[0]);
  *device_controller_remote = zx::channel(handles[1]);
  status = zx_handle_close(handles[2]);
  ASSERT_OK(status);

  // Validate the CreateDevice request.
  auto hdr = reinterpret_cast<fidl_message_header_t*>(bytes);
  ASSERT_EQ(fuchsia_device_manager_DevhostControllerCreateDeviceGenOrdinal, hdr->ordinal);
  status = fidl_decode(&fuchsia_device_manager_DevhostControllerCreateDeviceRequestTable, bytes,
                       actual_bytes, handles, actual_handles, nullptr);
  ASSERT_OK(status);
  auto req = reinterpret_cast<fuchsia_device_manager_DevhostControllerCreateDeviceRequest*>(bytes);
  ASSERT_EQ(req->driver_path.size, strlen(expected_driver));
  ASSERT_BYTES_EQ(reinterpret_cast<const uint8_t*>(expected_driver),
                  reinterpret_cast<const uint8_t*>(req->driver_path.data), req->driver_path.size,
                  "");
}

// Reads a Suspend request from remote and checks that it is for the expected
// flags, without sending a response. |SendSuspendReply| can be used to send the desired response.
void MultipleDeviceTestCase::CheckSuspendReceived(const zx::channel& remote,
                                                  uint32_t expected_flags, zx_txid_t* txid) {
  // Read the Suspend request.
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  zx_status_t status = remote.read(0, bytes, handles, sizeof(bytes), fbl::count_of(handles),
                                   &actual_bytes, &actual_handles);
  ASSERT_OK(status);
  ASSERT_LT(0, actual_bytes);
  ASSERT_EQ(0, actual_handles);

  // Validate the Suspend request.
  auto hdr = reinterpret_cast<fidl_message_header_t*>(bytes);
  *txid = hdr->txid;
  ASSERT_EQ(fuchsia_device_manager_DeviceControllerSuspendGenOrdinal, hdr->ordinal);
  status = fidl_decode(&fuchsia_device_manager_DeviceControllerSuspendRequestTable, bytes,
                       actual_bytes, handles, actual_handles, nullptr);
  ASSERT_OK(status);
  auto req = reinterpret_cast<fuchsia_device_manager_DeviceControllerSuspendRequest*>(bytes);
  ASSERT_EQ(req->flags, expected_flags);
}

// Sends a response with the given return_status. This can be used to reply to a
// request received by |CheckSuspendReceived|.
void MultipleDeviceTestCase::SendSuspendReply(const zx::channel& remote, zx_status_t return_status,
                                              zx_txid_t txid) {
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_handles;

  // Write the Suspend response.
  memset(bytes, 0, sizeof(bytes));
  auto resp = reinterpret_cast<fuchsia_device_manager_DeviceControllerSuspendResponse*>(bytes);
  fidl_init_txn_header(&resp->hdr, txid, fuchsia_device_manager_DeviceControllerSuspendGenOrdinal);
  resp->status = return_status;
  zx_status_t status =
      fidl_encode(&fuchsia_device_manager_DeviceControllerSuspendResponseTable, bytes,
                  sizeof(*resp), handles, fbl::count_of(handles), &actual_handles, nullptr);
  ASSERT_OK(status);
  ASSERT_EQ(0, actual_handles);
  status = remote.write(0, bytes, sizeof(*resp), nullptr, 0);
  ASSERT_OK(status);
}

// Reads a Suspend request from remote, checks that it is for the expected
// flags, and then sends the given response.
void MultipleDeviceTestCase::CheckSuspendReceivedAndReply(const zx::channel& remote,
                                                          uint32_t expected_flags,
                                                          zx_status_t return_status) {
  zx_txid_t txid;
  CheckSuspendReceived(remote, expected_flags, &txid);
  SendSuspendReply(remote, return_status, txid);
}

void MultipleDeviceTestCase::SetUp() {
  ASSERT_NO_FATAL_FAILURES(InitializeCoordinator(&coordinator_));

  // refcount starts at zero, so bump it up to keep us from being cleaned up
  devhost_.AddRef();
  {
    zx::channel local;
    zx_status_t status = zx::channel::create(0, &local, &devhost_remote_);
    ASSERT_OK(status);
    devhost_.set_hrpc(local.release());
  }

  // Set up the sys device proxy, inside of the devhost
  ASSERT_OK(coordinator_.PrepareProxy(coordinator_.sys_device(), &devhost_));
  coordinator_loop_.RunUntilIdle();
  ASSERT_NO_FATAL_FAILURES(CheckCreateDeviceReceived(devhost_remote_, kSystemDriverPath,
                                                     &sys_proxy_coordinator_remote_,
                                                     &sys_proxy_controller_remote_));
  coordinator_loop_.RunUntilIdle();

  // Create a child of the sys_device (an equivalent of the platform bus)
  {
    zx::channel local;
    zx_status_t status = zx::channel::create(0, &local, &platform_bus_.controller_remote);
    ASSERT_OK(status);

    zx::channel local2;
    status = zx::channel::create(0, &local2, &platform_bus_.coordinator_remote);
    ASSERT_OK(status);

    status = coordinator_.AddDevice(
        coordinator_.sys_device()->proxy(), std::move(local), std::move(local2),
        nullptr /* props_data */, 0 /* props_count */, "platform-bus", 0, nullptr /* driver_path */,
        nullptr /* args */, false /* invisible */, false /* has_init */, true /* always_init */,
        zx::channel() /* client_remote */, &platform_bus_.device);
    ASSERT_OK(status);
    coordinator_loop_.RunUntilIdle();

    ASSERT_NO_FATAL_FAILURES(CheckInitReceivedAndReply(platform_bus_.controller_remote));
    coordinator_loop()->RunUntilIdle();
  }
}

void MultipleDeviceTestCase::TearDown() {
  if (!coordinator_loop_thread_running_) {
    coordinator_loop_.RunUntilIdle();
  }
  // Remove the devices in the opposite order that we added them
  while (!devices_.is_empty()) {
    devices_.pop_back();
    if (!coordinator_loop_thread_running_) {
      coordinator_loop_.RunUntilIdle();
    }
  }
  platform_bus_.device.reset();
  if (!coordinator_loop_thread_running_) {
    coordinator_loop_.RunUntilIdle();
  }

  devhost_.devices().clear();
  // We no longer need the async loop.
  // If we do not shutdown here, the destructor
  // could be cleaning up the vfs, before the loop clears the
  // connections.
  coordinator_loop_.Shutdown();
}

void MultipleDeviceTestCase::AddDevice(const fbl::RefPtr<devmgr::Device>& parent, const char* name,
                                       uint32_t protocol_id, fbl::String driver, bool invisible,
                                       bool has_init, bool reply_to_init, bool always_init,
                                       size_t* index) {
  DeviceState state;

  zx::channel local, local2;
  zx_status_t status = zx::channel::create(0, &local, &state.controller_remote);
  ASSERT_OK(status);

  status = zx::channel::create(0, &local2, &state.coordinator_remote);
  ASSERT_OK(status);

  status = coordinator_.AddDevice(
      parent, std::move(local), std::move(local2), nullptr /* props_data */, 0 /* props_count */,
      name, protocol_id, driver.data() /* driver_path */, nullptr /* args */, invisible, has_init,
      always_init, zx::channel() /* client_remote */, &state.device);
  state.device->flags |= DEV_CTX_ALLOW_MULTI_COMPOSITE;
  ASSERT_OK(status);
  coordinator_loop_.RunUntilIdle();

  devices_.push_back(std::move(state));
  *index = devices_.size() - 1;

  if (reply_to_init) {
    ASSERT_NO_FATAL_FAILURES(CheckInitReceivedAndReply(device(*index)->controller_remote));
    coordinator_loop()->RunUntilIdle();
  }
}

void MultipleDeviceTestCase::AddDevice(const fbl::RefPtr<devmgr::Device>& parent, const char* name,
                                       uint32_t protocol_id, fbl::String driver, size_t* index) {
  AddDevice(parent, name, protocol_id, driver, false /* invisible */, false /* has_init */,
            true /* reply_to_init */, true /* always_init */, index);
}

void MultipleDeviceTestCase::RemoveDevice(size_t device_index) {
  auto& state = devices_[device_index];
  ASSERT_OK(coordinator_.RemoveDevice(state.device, false));
  state.device.reset();
  state.controller_remote.reset();
  state.coordinator_remote.reset();
  coordinator_loop_.RunUntilIdle();
}

bool MultipleDeviceTestCase::DeviceHasPendingMessages(const zx::channel& remote) {
  return remote.wait_one(ZX_CHANNEL_READABLE, zx::time(0), nullptr) == ZX_OK;
}
bool MultipleDeviceTestCase::DeviceHasPendingMessages(size_t device_index) {
  return DeviceHasPendingMessages(devices_[device_index].controller_remote);
}

void MultipleDeviceTestCase::DoSuspend(uint32_t flags,
                                       fit::function<void(uint32_t flags)> suspend_cb) {
  const bool vfs_exit_expected = (flags != DEVICE_SUSPEND_FLAG_SUSPEND_RAM);
  if (vfs_exit_expected) {
    zx::unowned_event event(coordinator()->fshost_event());
    auto thrd_func = [](void* ctx) -> int {
      zx::unowned_event event(*static_cast<zx::unowned_event*>(ctx));
      if (event->wait_one(FSHOST_SIGNAL_EXIT, zx::time::infinite(), nullptr) != ZX_OK) {
        return false;
      }
      if (event->signal(0, FSHOST_SIGNAL_EXIT_DONE) != ZX_OK) {
        return false;
      }
      return true;
    };

    thrd_t fshost_thrd;
    ASSERT_EQ(thrd_create(&fshost_thrd, thrd_func, &event), thrd_success);

    suspend_cb(flags);
    if (!coordinator_loop_thread_running()) {
      coordinator_loop()->RunUntilIdle();
    }
    int thread_status;
    ASSERT_EQ(thrd_join(fshost_thrd, &thread_status), thrd_success);
    ASSERT_TRUE(thread_status);

    // Make sure that vfs_exit() happened.
    ASSERT_OK(
        coordinator()->fshost_event().wait_one(FSHOST_SIGNAL_EXIT_DONE, zx::time(0), nullptr));
  } else {
    suspend_cb(flags);
    if (!coordinator_loop_thread_running()) {
      coordinator_loop()->RunUntilIdle();
    }

    // Make sure that vfs_exit() didn't happen.
    ASSERT_EQ(coordinator()->fshost_event().wait_one(FSHOST_SIGNAL_EXIT | FSHOST_SIGNAL_EXIT_DONE,
                                                     zx::time(0), nullptr),
              ZX_ERR_TIMED_OUT);
  }
}

void MultipleDeviceTestCase::DoSuspend(uint32_t flags) {
  DoSuspend(flags, [this](uint32_t flags) { coordinator()->Suspend(flags); });
}

// Reads the request from |remote| and verifies whether it matches the expected Unbind request.
// |SendUnbindReply| can be used to send the desired response.
void MultipleDeviceTestCase::CheckUnbindReceived(const zx::channel& remote, zx_txid_t* txid) {
  // Read the unbind request.
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  zx_status_t status = remote.read(0, bytes, handles, sizeof(bytes), fbl::count_of(handles),
                                   &actual_bytes, &actual_handles);
  ASSERT_OK(status);
  ASSERT_LT(0, actual_bytes);
  ASSERT_EQ(0, actual_handles);

  // Validate the unbind request.
  auto hdr = reinterpret_cast<fidl_message_header_t*>(bytes);
  *txid = hdr->txid;
  ASSERT_EQ(fuchsia_device_manager_DeviceControllerUnbindGenOrdinal, hdr->ordinal);
  status = fidl_decode(&fuchsia_device_manager_DeviceControllerUnbindRequestTable, bytes,
                       actual_bytes, handles, actual_handles, nullptr);
  ASSERT_OK(status);
}

// Sends a response with the given return_status. This can be used to reply to a
// request received by |CheckUnbindReceived|.
void MultipleDeviceTestCase::SendUnbindReply(const zx::channel& remote, zx_txid_t txid) {
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_handles;
  memset(bytes, 0, sizeof(bytes));
  auto resp = reinterpret_cast<fuchsia_device_manager_DeviceControllerUnbindResponse*>(bytes);
  fidl_init_txn_header(&resp->hdr, txid, fuchsia_device_manager_DeviceControllerUnbindGenOrdinal);
  resp->result = fuchsia_device_manager_DeviceController_Unbind_Result{
      .tag = 0,
      .response = {},
  };
  auto status =
      fidl_encode(&fuchsia_device_manager_DeviceControllerUnbindResponseTable, bytes, sizeof(*resp),
                  handles, fbl::count_of(handles), &actual_handles, nullptr);
  ASSERT_OK(status);
  ASSERT_EQ(0, actual_handles);
  status = remote.write(0, bytes, sizeof(*resp), nullptr, 0);
  ASSERT_OK(status);
}

void MultipleDeviceTestCase::CheckUnbindReceivedAndReply(const zx::channel& remote) {
  zx_txid_t txid;
  CheckUnbindReceived(remote, &txid);
  SendUnbindReply(remote, txid);
}

// Reads the request from |remote| and verifies whether it matches the expected
// Unbind request.
// |SendRemoveReply| can be used to send the desired response.
void MultipleDeviceTestCase::CheckRemoveReceived(const zx::channel& remote, zx_txid_t* txid) {
  // Read the remove request.
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  zx_status_t status = remote.read(0, bytes, handles, sizeof(bytes), fbl::count_of(handles),
                                   &actual_bytes, &actual_handles);
  ASSERT_OK(status);
  ASSERT_LT(0, actual_bytes);
  ASSERT_EQ(0, actual_handles);

  // Validate the remove request.
  auto hdr = reinterpret_cast<fidl_message_header_t*>(bytes);
  ASSERT_EQ(fuchsia_device_manager_DeviceControllerCompleteRemovalGenOrdinal, hdr->ordinal);
  status = fidl_decode(&fuchsia_device_manager_DeviceControllerCompleteRemovalRequestTable, bytes,
                       actual_bytes, handles, actual_handles, nullptr);
  *txid = hdr->txid;
  ASSERT_OK(status);
}

// Sends a response with the given return_status. This can be used to reply to a
// request received by |CheckRemoveReceived|.
void MultipleDeviceTestCase::SendRemoveReply(const zx::channel& remote, zx_txid_t txid) {
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_handles;
  memset(bytes, 0, sizeof(bytes));
  auto resp =
      reinterpret_cast<fuchsia_device_manager_DeviceControllerCompleteRemovalResponse*>(bytes);
  fidl_init_txn_header(&resp->hdr, txid,
                       fuchsia_device_manager_DeviceControllerCompleteRemovalGenOrdinal);
  resp->result = fuchsia_device_manager_DeviceController_CompleteRemoval_Result{
      .tag = 0,
      .response = {},
  };
  auto status =
      fidl_encode(&fuchsia_device_manager_DeviceControllerCompleteRemovalResponseTable, bytes,
                  sizeof(*resp), handles, fbl::count_of(handles), &actual_handles, nullptr);
  ASSERT_OK(status);
  ASSERT_EQ(0, actual_handles);
  status = remote.write(0, bytes, sizeof(*resp), nullptr, 0);
  ASSERT_OK(status);
}

void MultipleDeviceTestCase::CheckRemoveReceivedAndReply(const zx::channel& remote) {
  zx_txid_t txid;
  CheckRemoveReceived(remote, &txid);
  SendRemoveReply(remote, txid);
}

// Reads a Resume request from remote and checks that it is for the expected
// target state, without sending a response. |SendResumeReply| can be used to send the desired
// response.

void MultipleDeviceTestCase::CheckResumeReceived(const zx::channel& remote,
                                                 SystemPowerState target_state, zx_txid_t* txid) {
  // Read the Resume request.
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  zx_status_t status = remote.read(0, bytes, handles, sizeof(bytes), fbl::count_of(handles),
                                   &actual_bytes, &actual_handles);
  ASSERT_OK(status);
  ASSERT_LT(0, actual_bytes);
  ASSERT_EQ(0, actual_handles);

  // Validate the Resume request.
  auto hdr = reinterpret_cast<fidl_message_header_t*>(bytes);
  *txid = hdr->txid;
  ASSERT_EQ(fuchsia_device_manager_DeviceControllerResumeGenOrdinal, hdr->ordinal);
  status = fidl_decode(&fuchsia_device_manager_DeviceControllerResumeRequestTable, bytes,
                       actual_bytes, handles, actual_handles, nullptr);
  ASSERT_OK(status);
  auto req = reinterpret_cast<fuchsia_device_manager_DeviceControllerResumeRequest*>(bytes);
  ASSERT_EQ(static_cast<SystemPowerState>(req->target_system_state), target_state);
}

// Sends a response with the given return_status. This can be used to reply to a
// request received by |CheckResumeReceived|.
void MultipleDeviceTestCase::SendResumeReply(const zx::channel& remote, zx_status_t return_status,
                                             zx_txid_t txid) {
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_handles;

  // Write the Resume response.
  memset(bytes, 0, sizeof(bytes));
  auto resp = reinterpret_cast<fuchsia_device_manager_DeviceControllerResumeResponse*>(bytes);
  fidl_init_txn_header(&resp->hdr, txid, fuchsia_device_manager_DeviceControllerResumeGenOrdinal);
  resp->status = return_status;
  zx_status_t status =
      fidl_encode(&fuchsia_device_manager_DeviceControllerResumeResponseTable, bytes, sizeof(*resp),
                  handles, fbl::count_of(handles), &actual_handles, nullptr);
  ASSERT_OK(status);
  ASSERT_EQ(0, actual_handles);
  status = remote.write(0, bytes, sizeof(*resp), nullptr, 0);
  ASSERT_OK(status);
}

// Reads a Resume request from remote, checks that it is for the expected
// target state, and then sends the given response.
void MultipleDeviceTestCase::CheckResumeReceived(const zx::channel& remote,
                                                 SystemPowerState target_state,
                                                 zx_status_t return_status) {
  zx_txid_t txid;
  CheckResumeReceived(remote, target_state, &txid);
  SendResumeReply(remote, return_status, txid);
}

void MultipleDeviceTestCase::DoResume(
    SystemPowerState target_state, fit::function<void(SystemPowerState target_state)> resume_cb) {
  resume_cb(target_state);
  if (!coordinator_loop_thread_running()) {
    coordinator_loop()->RunUntilIdle();
  }
}

void MultipleDeviceTestCase::DoResume(SystemPowerState target_state,
                                      devmgr::ResumeCallback callback) {
  DoResume(target_state, [this, callback = std::move(callback)](SystemPowerState target_state) {
    coordinator()->Resume(target_state, callback);
  });
}
// Reads the request from |remote| and verifies whether it matches the expected Init request.
// |SendInitReply| can be used to send the desired response.
void MultipleDeviceTestCase::CheckInitReceived(const zx::channel& remote, zx_txid_t* txid) {
  // Read the init request.
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  zx_status_t status = remote.read(0, bytes, handles, sizeof(bytes), fbl::count_of(handles),
                                   &actual_bytes, &actual_handles);
  ASSERT_OK(status);
  ASSERT_LT(0, actual_bytes);
  ASSERT_EQ(0, actual_handles);

  // Validate the init request.
  auto hdr = reinterpret_cast<fidl_message_header_t*>(bytes);
  *txid = hdr->txid;
  ASSERT_EQ(fuchsia_device_manager_DeviceControllerInitGenOrdinal, hdr->ordinal);
  status = fidl_decode(&fuchsia_device_manager_DeviceControllerInitRequestTable, bytes,
                       actual_bytes, handles, actual_handles, nullptr);
  ASSERT_OK(status);
}

// Sends a response with the given return_status. This can be used to reply to a
// request received by |CheckInitializingReceived|.
void MultipleDeviceTestCase::SendInitReply(const zx::channel& remote, zx_txid_t txid,
                                           zx_status_t return_status) {
  FIDL_ALIGNDECL uint8_t bytes[ZX_CHANNEL_MAX_MSG_BYTES];
  zx_handle_t handles[ZX_CHANNEL_MAX_MSG_HANDLES];
  uint32_t actual_handles;

  // Write the Resume response.
  memset(bytes, 0, sizeof(bytes));
  auto resp = reinterpret_cast<fuchsia_device_manager_DeviceControllerInitResponse*>(bytes);
  fidl_init_txn_header(&resp->hdr, txid, fuchsia_device_manager_DeviceControllerInitOrdinal);
  resp->status = return_status;
  zx_status_t status =
      fidl_encode(&fuchsia_device_manager_DeviceControllerInitResponseTable, bytes, sizeof(*resp),
                  handles, fbl::count_of(handles), &actual_handles, nullptr);
  ASSERT_OK(status);
  ASSERT_EQ(0, actual_handles);
  status = remote.write(0, bytes, sizeof(*resp), nullptr, 0);
  ASSERT_OK(status);
}

void MultipleDeviceTestCase::CheckInitReceivedAndReply(const zx::channel& remote,
                                                       zx_status_t return_status) {
  zx_txid_t txid;
  CheckInitReceived(remote, &txid);
  SendInitReply(remote, txid, return_status);
}
