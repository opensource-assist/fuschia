// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_CONNECTIVITY_BLUETOOTH_CORE_BT_HOST_GAP_PAIRING_STATE_H_
#define SRC_CONNECTIVITY_BLUETOOTH_CORE_BT_HOST_GAP_PAIRING_STATE_H_

#include <fbl/macros.h>

#include <optional>

#include "src/connectivity/bluetooth/core/bt-host/hci/hci.h"
#include "src/connectivity/bluetooth/core/bt-host/sm/smp.h"

namespace bt {
namespace gap {

// Represents the local user interaction that will occur, as inferred from Core
// Spec v5.0 Vol 3, Part C, Sec 5.2.2.6 (Table 5.7). This is not directly
// coupled to the reply action for the HCI "User" event for pairing; e.g.
// kDisplayPasskey may mean automatically confirming User Confirmation Request
// or displaying the value from User Passkey Notification.
enum class PairingAction {
  // Don't involve the user.
  kAutomatic,

  // Request yes/no consent.
  kGetConsent,

  // Display 6-digit value with "cancel."
  kDisplayPasskey,

  // Display 6-digit value with "yes/no."
  kComparePasskey,

  // Request a 6-digit value entry.
  kRequestPasskey,
};

// This class is not thread-safe and should only be called on the thread on
// which it was created.
class PairingState final {
 public:
  // Used to report the status of a pairing procedure. |status| will contain
  // HostError::kNotSupported if the pairing procedure does not proceed in the
  // order of events expected.
  using StatusCallback =
      fit::function<void(hci::ConnectionHandle, hci::Status)>;
  PairingState(StatusCallback status_cb);
  ~PairingState() = default;

  // True if there is currently a pairing procedure in progress that the local
  // device initiated.
  bool initiator() const { return initiator_; }

  // Starts pairing against the peer, if pairing is not already in progress.
  // If not, this device becomes the pairing initiator, and returns
  // |kSendAuthenticationRequest| to indicate that the caller shall send an
  // Authentication Request for this peer.
  enum class InitiatorAction {
    kDoNotSendAuthenticationRequest,
    kSendAuthenticationRequest,
  };
  [[nodiscard]] InitiatorAction InitiatePairing();

  // Event handlers. Caller must ensure that the event is addressed to the link
  // for this PairingState.

  // Returns value for IO Capability Request Reply, else std::nullopt for IO
  // Capability Negative Reply.
  //
  // TODO(BT-8): Indicate presence of out-of-band (OOB) data.
  [[nodiscard]] std::optional<hci::IOCapability> OnIoCapabilityRequest();

  // Caller is not expected to send a response.
  void OnIoCapabilityResponse(hci::IOCapability peer_iocap);

  // |cb| is called with: true to send User Confirmation Request Reply, else
  // for to send User Confirmation Request Negative Reply.
  using UserConfirmationCallback = fit::callback<void(bool confirm)>;
  void OnUserConfirmationRequest(uint32_t numeric_value,
                                 UserConfirmationCallback cb);

  // |cb| is called with: passkey value to send User Passkey Request Reply, else
  // std::nullopt to send User Passkey Request Negative Reply.
  using UserPasskeyCallback =
      fit::callback<void(std::optional<uint32_t> passkey)>;
  void OnUserPasskeyRequest(UserPasskeyCallback cb);

  // Caller is not expected to send a response.
  void OnUserPasskeyNotification(uint32_t numeric_value);

  // Caller is not expected to send a response.
  void OnSimplePairingComplete(hci::StatusCode status_code);

  // Caller is not expected to send a response.
  void OnLinkKeyNotification(const UInt128& link_key,
                             hci::LinkKeyType key_type);

  // Caller is not expected to send a response.
  void OnAuthenticationComplete(hci::StatusCode status_code);

  // Handler for hci::Connection::set_encryption_change_callback.
  void OnEncryptionChange(hci::Status status, bool enabled);

 private:
  enum class State {
    // Wait for initiator's IO Capability Response or for locally-initiated
    // pairing.
    kIdle,

    // As initiator, wait for IO Capability Request or Authentication Complete.
    kInitiatorPairingStarted,

    // As initiator, wait for IO Capability Response.
    kInitiatorWaitIoCapResponse,

    // As responder, wait for IO Capability Request.
    kResponderWaitIoCapRequest,

    // Wait for controller event for pairing action.
    kWaitPairingEvent,

    // Wait for Simple Pairing Complete.
    kWaitPairingComplete,

    // Wait for Link Key Notification.
    kWaitLinkKey,

    // As initiator, wait for Authentication Complete.
    kInitiatorWaitAuthComplete,

    // Wait for Encryption Change.
    kWaitEncryption,

    // Error occurred; wait for link closure and ignore events.
    kFailed,
  };

  static const char* ToString(State state);

  State state() const { return state_; }

  // Called to enable encryption on the link for this peer. Sets |state_| to
  // kWaitEncryption.
  void EnableEncryption();

  // Called when an event is received while in a state that doesn't expect that
  // event. Invokes |status_callback_| with HostError::kNotSupported and sets
  // |state_| to kFailed.
  void FailWithUnexpectedEvent();

  // Set to true by locally-initiated pairing and cleared when pairing is reset.
  bool initiator_;

  // State machine representation.
  State state_;

  // Holds the callback that this object was constructed with.
  StatusCallback status_callback_;

  DISALLOW_COPY_AND_ASSIGN_ALLOW_MOVE(PairingState);
};

PairingAction GetInitiatorPairingAction(hci::IOCapability initiator_cap,
                                        hci::IOCapability responder_cap);
PairingAction GetResponderPairingAction(hci::IOCapability initiator_cap,
                                        hci::IOCapability responder_cap);
hci::EventCode GetExpectedEvent(hci::IOCapability local_cap,
                                hci::IOCapability peer_cap);
bool IsPairingAuthenticated(hci::IOCapability local_cap,
                            hci::IOCapability peer_cap);

}  // namespace gap
}  // namespace bt

#endif  // SRC_CONNECTIVITY_BLUETOOTH_CORE_BT_HOST_GAP_PAIRING_STATE_H_
