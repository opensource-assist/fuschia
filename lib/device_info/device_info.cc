// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "apps/modular/lib/device_info/device_info.h"

#include <limits.h>
#include <unistd.h>

#include "apps/modular/lib/util/filesystem.h"
#include "lib/ftl/files/directory.h"
#include "lib/ftl/files/file.h"
#include "lib/ftl/logging.h"
#include "lib/ftl/macros.h"
#include "lib/ftl/random/uuid.h"
#include "lib/ftl/strings/string_printf.h"
#include "lib/ftl/strings/trim.h"

namespace modular {

constexpr char kDeviceInfoDirectory[] = "/data/modular/device";
constexpr char kDeviceIDFile[] = "/data/modular/device/%s.syncid";
constexpr char kDeviceNameFile[] = "/data/modular/device/%s.devicename";
constexpr char kSyncDeviceProfile[] =
    "/data/modular/device/profile_config.json";

std::string LoadDeviceProfile() {
  std::string device_profile;

  if (!files::IsDirectory(kDeviceInfoDirectory)) {
    files::CreateDirectory(kDeviceInfoDirectory);
  }

  if (!files::ReadFileToString(kSyncDeviceProfile, &device_profile)) {
    device_profile = "{}";
  }

  return device_profile;
}

// TODO(security): this is a temporary implementation. audit the revocability
// of this ID for syncing.
// TODO(zbowling): refactor to use flatbuffer for storage
std::string LoadDeviceID(const std::string& user) {
  std::string device_id;

  if (!files::IsDirectory(kDeviceInfoDirectory)) {
    files::CreateDirectory(kDeviceInfoDirectory);
  }

  // FIXME(zbowling): this isn't scalable
  std::string path = ftl::StringPrintf(kDeviceIDFile, user.c_str());

  if (!files::ReadFileToString(path, &device_id)) {
    // no existing device id. generate a UUID and store it to disk
    device_id = ftl::GenerateUUID();
    bool success = files::WriteFile(path, device_id.data(), device_id.length());
    FTL_DCHECK(success);
  }

  FTL_LOG(INFO) << "device_info: syncing device id for user: " << user
                << "   set to: " << device_id;

  return device_id;
}

// TODO(zbowling): implement WriteDeviceName
std::string LoadDeviceName(const std::string& user) {
  std::string device_name;

  if (!files::IsDirectory(kDeviceInfoDirectory)) {
    files::CreateDirectory(kDeviceInfoDirectory);
  }

  std::string path = ftl::StringPrintf(kDeviceNameFile, user.c_str());

  if (files::ReadFileToString(path, &device_name)) {
    // Remove whitespace because vim and echo like adding a newline.
    constexpr char ws[] = " \t\n\r";
    device_name = ftl::TrimString(device_name, ws).ToString();
  }

  if (device_name.empty()) {
    // gethostname() will return "fuchsia" if the network stack hasn't started.
    // Generally by this point we should have used OAuth to auth. This code is
    // designed to allow a friendly device name, or a fallback to the result
    // of gethostname() if the user didn't change it. The friendly name is
    // defined by users, so there appears to be no requirement for uniqueness.
    char host_name_buffer[HOST_NAME_MAX + 1];
    // Defense in depth.
    host_name_buffer[HOST_NAME_MAX] = '\0';
    int result = gethostname(host_name_buffer, sizeof(host_name_buffer));

    if (result < 0) {
      FTL_LOG(ERROR) << "unable to get hostname. errno " << errno;
      device_name = "fuchsia";
    } else {
      device_name = host_name_buffer;
    }

    // Saving this value causes any changes in the return value of gethostname()
    // to be ignored, but the cached copy helps with the aforementioned problem
    // that the network stack may not have started.
    // TODO(jimbe) Don't write the result of gethostname() to this file once
    // NET-79 is fixed. (Maybe write an empty file so users can find it.)
    bool success =
        files::WriteFile(path, device_name.data(), device_name.length());
    FTL_DCHECK(success);
  }

  return device_name;
}

}  // namespace modular
