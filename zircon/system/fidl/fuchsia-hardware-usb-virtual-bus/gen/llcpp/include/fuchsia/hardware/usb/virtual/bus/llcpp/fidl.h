// WARNING: This file is machine generated by fidlgen.

#pragma once

#include <lib/fidl/internal.h>
#include <lib/fidl/txn_header.h>
#include <lib/fidl/llcpp/array.h>
#include <lib/fidl/llcpp/coding.h>
#include <lib/fidl/llcpp/connect_service.h>
#include <lib/fidl/llcpp/service_handler_interface.h>
#include <lib/fidl/llcpp/string_view.h>
#include <lib/fidl/llcpp/sync_call.h>
#include <lib/fidl/llcpp/traits.h>
#include <lib/fidl/llcpp/transaction.h>
#include <lib/fidl/llcpp/vector_view.h>
#include <lib/fit/function.h>
#include <lib/zx/channel.h>
#include <zircon/fidl.h>

namespace llcpp {

namespace fuchsia {
namespace hardware {
namespace usb {
namespace virtual_ {
namespace bus {

class Bus;

extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusEnableRequestTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusEnableRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusEnableResponseTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusEnableResponseTable;
extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusDisableRequestTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusDisableRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusDisableResponseTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusDisableResponseTable;
extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusConnectRequestTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusConnectRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusConnectResponseTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusConnectResponseTable;
extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusDisconnectRequestTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusDisconnectRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_usb_virtual_bus_BusDisconnectResponseTable;
extern "C" const fidl_type_t v1_fuchsia_hardware_usb_virtual_bus_BusDisconnectResponseTable;

class Bus final {
  Bus() = delete;
 public:

  struct EnableResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;

    static constexpr const fidl_type_t* Type = &v1_fuchsia_hardware_usb_virtual_bus_BusEnableResponseTable;
    static constexpr const fidl_type_t* AltType = &fuchsia_hardware_usb_virtual_bus_BusEnableResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr uint32_t AltPrimarySize = 24;
    static constexpr uint32_t AltMaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr bool ContainsUnion = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  using EnableRequest = ::fidl::AnyZeroArgMessage;

  struct DisableResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;

    static constexpr const fidl_type_t* Type = &v1_fuchsia_hardware_usb_virtual_bus_BusDisableResponseTable;
    static constexpr const fidl_type_t* AltType = &fuchsia_hardware_usb_virtual_bus_BusDisableResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr uint32_t AltPrimarySize = 24;
    static constexpr uint32_t AltMaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr bool ContainsUnion = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  using DisableRequest = ::fidl::AnyZeroArgMessage;

  struct ConnectResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;

    static constexpr const fidl_type_t* Type = &v1_fuchsia_hardware_usb_virtual_bus_BusConnectResponseTable;
    static constexpr const fidl_type_t* AltType = &fuchsia_hardware_usb_virtual_bus_BusConnectResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr uint32_t AltPrimarySize = 24;
    static constexpr uint32_t AltMaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr bool ContainsUnion = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  using ConnectRequest = ::fidl::AnyZeroArgMessage;

  struct DisconnectResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;

    static constexpr const fidl_type_t* Type = &v1_fuchsia_hardware_usb_virtual_bus_BusDisconnectResponseTable;
    static constexpr const fidl_type_t* AltType = &fuchsia_hardware_usb_virtual_bus_BusDisconnectResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr uint32_t AltPrimarySize = 24;
    static constexpr uint32_t AltMaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr bool ContainsUnion = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  using DisconnectRequest = ::fidl::AnyZeroArgMessage;


  // Collection of return types of FIDL calls in this interface.
  class ResultOf final {
    ResultOf() = delete;
   private:
    template <typename ResponseType>
    class Enable_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Enable_Impl(::zx::unowned_channel _client_end);
      ~Enable_Impl() = default;
      Enable_Impl(Enable_Impl&& other) = default;
      Enable_Impl& operator=(Enable_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Disable_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Disable_Impl(::zx::unowned_channel _client_end);
      ~Disable_Impl() = default;
      Disable_Impl(Disable_Impl&& other) = default;
      Disable_Impl& operator=(Disable_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Connect_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Connect_Impl(::zx::unowned_channel _client_end);
      ~Connect_Impl() = default;
      Connect_Impl(Connect_Impl&& other) = default;
      Connect_Impl& operator=(Connect_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Disconnect_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Disconnect_Impl(::zx::unowned_channel _client_end);
      ~Disconnect_Impl() = default;
      Disconnect_Impl(Disconnect_Impl&& other) = default;
      Disconnect_Impl& operator=(Disconnect_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };

   public:
    using Enable = Enable_Impl<EnableResponse>;
    using Disable = Disable_Impl<DisableResponse>;
    using Connect = Connect_Impl<ConnectResponse>;
    using Disconnect = Disconnect_Impl<DisconnectResponse>;
  };

  // Collection of return types of FIDL calls in this interface,
  // when the caller-allocate flavor or in-place call is used.
  class UnownedResultOf final {
    UnownedResultOf() = delete;
   private:
    template <typename ResponseType>
    class Enable_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Enable_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);
      ~Enable_Impl() = default;
      Enable_Impl(Enable_Impl&& other) = default;
      Enable_Impl& operator=(Enable_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Disable_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Disable_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);
      ~Disable_Impl() = default;
      Disable_Impl(Disable_Impl&& other) = default;
      Disable_Impl& operator=(Disable_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Connect_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Connect_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);
      ~Connect_Impl() = default;
      Connect_Impl(Connect_Impl&& other) = default;
      Connect_Impl& operator=(Connect_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Disconnect_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Disconnect_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);
      ~Disconnect_Impl() = default;
      Disconnect_Impl(Disconnect_Impl&& other) = default;
      Disconnect_Impl& operator=(Disconnect_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };

   public:
    using Enable = Enable_Impl<EnableResponse>;
    using Disable = Disable_Impl<DisableResponse>;
    using Connect = Connect_Impl<ConnectResponse>;
    using Disconnect = Disconnect_Impl<DisconnectResponse>;
  };

  class SyncClient final {
   public:
    explicit SyncClient(::zx::channel channel) : channel_(std::move(channel)) {}
    ~SyncClient() = default;
    SyncClient(SyncClient&&) = default;
    SyncClient& operator=(SyncClient&&) = default;

    const ::zx::channel& channel() const { return channel_; }

    ::zx::channel* mutable_channel() { return &channel_; }

    // Enables the virtual bus.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Enable Enable();

    // Enables the virtual bus.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Enable Enable(::fidl::BytePart _response_buffer);

    // Disables the virtual bus.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Disable Disable();

    // Disables the virtual bus.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Disable Disable(::fidl::BytePart _response_buffer);

    // Simulates a USB connected event.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Connect Connect();

    // Simulates a USB connected event.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Connect Connect(::fidl::BytePart _response_buffer);

    // Simulates a USB connected event.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Disconnect Disconnect();

    // Simulates a USB connected event.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Disconnect Disconnect(::fidl::BytePart _response_buffer);

   private:
    ::zx::channel channel_;
  };

  // Methods to make a sync FIDL call directly on an unowned channel, avoiding setting up a client.
  class Call final {
    Call() = delete;
   public:

    // Enables the virtual bus.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Enable Enable(::zx::unowned_channel _client_end);

    // Enables the virtual bus.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Enable Enable(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);

    // Disables the virtual bus.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Disable Disable(::zx::unowned_channel _client_end);

    // Disables the virtual bus.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Disable Disable(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);

    // Simulates a USB connected event.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Connect Connect(::zx::unowned_channel _client_end);

    // Simulates a USB connected event.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Connect Connect(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);

    // Simulates a USB connected event.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Disconnect Disconnect(::zx::unowned_channel _client_end);

    // Simulates a USB connected event.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Disconnect Disconnect(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);

  };

  // Messages are encoded and decoded in-place when these methods are used.
  // Additionally, requests must be already laid-out according to the FIDL wire-format.
  class InPlace final {
    InPlace() = delete;
   public:

    // Enables the virtual bus.
    static ::fidl::DecodeResult<EnableResponse> Enable(::zx::unowned_channel _client_end, ::fidl::BytePart response_buffer);

    // Disables the virtual bus.
    static ::fidl::DecodeResult<DisableResponse> Disable(::zx::unowned_channel _client_end, ::fidl::BytePart response_buffer);

    // Simulates a USB connected event.
    static ::fidl::DecodeResult<ConnectResponse> Connect(::zx::unowned_channel _client_end, ::fidl::BytePart response_buffer);

    // Simulates a USB connected event.
    static ::fidl::DecodeResult<DisconnectResponse> Disconnect(::zx::unowned_channel _client_end, ::fidl::BytePart response_buffer);

  };

  // Pure-virtual interface to be implemented by a server.
  class Interface {
   public:
    Interface() = default;
    virtual ~Interface() = default;
    using _Outer = Bus;
    using _Base = ::fidl::CompleterBase;

    class EnableCompleterBase : public _Base {
     public:
      void Reply(int32_t status);
      void Reply(::fidl::BytePart _buffer, int32_t status);
      void Reply(::fidl::DecodedMessage<EnableResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using EnableCompleter = ::fidl::Completer<EnableCompleterBase>;

    virtual void Enable(EnableCompleter::Sync _completer) = 0;

    class DisableCompleterBase : public _Base {
     public:
      void Reply(int32_t status);
      void Reply(::fidl::BytePart _buffer, int32_t status);
      void Reply(::fidl::DecodedMessage<DisableResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using DisableCompleter = ::fidl::Completer<DisableCompleterBase>;

    virtual void Disable(DisableCompleter::Sync _completer) = 0;

    class ConnectCompleterBase : public _Base {
     public:
      void Reply(int32_t status);
      void Reply(::fidl::BytePart _buffer, int32_t status);
      void Reply(::fidl::DecodedMessage<ConnectResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using ConnectCompleter = ::fidl::Completer<ConnectCompleterBase>;

    virtual void Connect(ConnectCompleter::Sync _completer) = 0;

    class DisconnectCompleterBase : public _Base {
     public:
      void Reply(int32_t status);
      void Reply(::fidl::BytePart _buffer, int32_t status);
      void Reply(::fidl::DecodedMessage<DisconnectResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using DisconnectCompleter = ::fidl::Completer<DisconnectCompleterBase>;

    virtual void Disconnect(DisconnectCompleter::Sync _completer) = 0;

  };

  // Attempts to dispatch the incoming message to a handler function in the server implementation.
  // If there is no matching handler, it returns false, leaving the message and transaction intact.
  // In all other cases, it consumes the message and returns true.
  // It is possible to chain multiple TryDispatch functions in this manner.
  static bool TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn);

  // Dispatches the incoming message to one of the handlers functions in the interface.
  // If there is no matching handler, it closes all the handles in |msg| and closes the channel with
  // a |ZX_ERR_NOT_SUPPORTED| epitaph, before returning false. The message should then be discarded.
  static bool Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn);

  // Same as |Dispatch|, but takes a |void*| instead of |Interface*|. Only used with |fidl::Bind|
  // to reduce template expansion.
  // Do not call this method manually. Use |Dispatch| instead.
  static bool TypeErasedDispatch(void* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
    return Dispatch(static_cast<Interface*>(impl), msg, txn);
  }


  // Helper functions to fill in the transaction header in a |DecodedMessage<TransactionalMessage>|.
  class SetTransactionHeaderFor final {
    SetTransactionHeaderFor() = delete;
   public:
    static void EnableRequest(const ::fidl::DecodedMessage<Bus::EnableRequest>& _msg);
    static void EnableResponse(const ::fidl::DecodedMessage<Bus::EnableResponse>& _msg);
    static void DisableRequest(const ::fidl::DecodedMessage<Bus::DisableRequest>& _msg);
    static void DisableResponse(const ::fidl::DecodedMessage<Bus::DisableResponse>& _msg);
    static void ConnectRequest(const ::fidl::DecodedMessage<Bus::ConnectRequest>& _msg);
    static void ConnectResponse(const ::fidl::DecodedMessage<Bus::ConnectResponse>& _msg);
    static void DisconnectRequest(const ::fidl::DecodedMessage<Bus::DisconnectRequest>& _msg);
    static void DisconnectResponse(const ::fidl::DecodedMessage<Bus::DisconnectResponse>& _msg);
  };
};

}  // namespace bus
}  // namespace virtual_
}  // namespace usb
}  // namespace hardware
}  // namespace fuchsia
}  // namespace llcpp

namespace fidl {

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::EnableResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::EnableResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::EnableResponse)
    == ::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::EnableResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::EnableResponse, status) == 16);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisableResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisableResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisableResponse)
    == ::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisableResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisableResponse, status) == 16);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::ConnectResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::ConnectResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::ConnectResponse)
    == ::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::ConnectResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::ConnectResponse, status) == 16);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisconnectResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisconnectResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisconnectResponse)
    == ::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisconnectResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::usb::virtual_::bus::Bus::DisconnectResponse, status) == 16);

}  // namespace fidl
