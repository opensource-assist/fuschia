// WARNING: This file is machine generated by fidlgen.

#include <fuchsia/buttons/llcpp/fidl.h>
#include <memory>

namespace llcpp {

namespace fuchsia {
namespace buttons {

void ::llcpp::fuchsia::buttons::Buttons_RegisterNotify_Result::SizeAndOffsetAssertionHelper() {
  static_assert(sizeof(Buttons_RegisterNotify_Result) == sizeof(fidl_xunion_t));
  static_assert(offsetof(Buttons_RegisterNotify_Result, ordinal_) == offsetof(fidl_xunion_t, tag));
  static_assert(offsetof(Buttons_RegisterNotify_Result, envelope_) == offsetof(fidl_xunion_t, envelope));
}

namespace {

[[maybe_unused]]
constexpr uint64_t kButtons_GetState_Ordinal = 0x3f5c2ec300000000lu;
[[maybe_unused]]
constexpr uint64_t kButtons_GetState_GenOrdinal = 0x10ee35ec461a178alu;
extern "C" const fidl_type_t fuchsia_buttons_ButtonsGetStateRequestTable;
extern "C" const fidl_type_t fuchsia_buttons_ButtonsGetStateResponseTable;
extern "C" const fidl_type_t v1_fuchsia_buttons_ButtonsGetStateResponseTable;
[[maybe_unused]]
constexpr uint64_t kButtons_RegisterNotify_Ordinal = 0x9a78d6400000000lu;
[[maybe_unused]]
constexpr uint64_t kButtons_RegisterNotify_GenOrdinal = 0x1166530aab1f618blu;
extern "C" const fidl_type_t fuchsia_buttons_ButtonsRegisterNotifyRequestTable;
extern "C" const fidl_type_t fuchsia_buttons_ButtonsRegisterNotifyResponseTable;
extern "C" const fidl_type_t v1_fuchsia_buttons_ButtonsRegisterNotifyResponseTable;
[[maybe_unused]]
constexpr uint64_t kButtons_Notify_Ordinal = 0x551dd30100000000lu;
[[maybe_unused]]
constexpr uint64_t kButtons_Notify_GenOrdinal = 0x49f747239d72f910lu;
extern "C" const fidl_type_t fuchsia_buttons_ButtonsNotifyRequestTable;
extern "C" const fidl_type_t fuchsia_buttons_ButtonsNotifyEventTable;
extern "C" const fidl_type_t v1_fuchsia_buttons_ButtonsNotifyEventTable;

}  // namespace
template <>
Buttons::ResultOf::GetState_Impl<Buttons::GetStateResponse>::GetState_Impl(::zx::unowned_channel _client_end, ::llcpp::fuchsia::buttons::ButtonType type) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetStateRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, GetStateRequest::PrimarySize);
  auto& _request = *reinterpret_cast<GetStateRequest*>(_write_bytes);
  _request.type = std::move(type);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(GetStateRequest));
  ::fidl::DecodedMessage<GetStateRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      Buttons::InPlace::GetState(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Buttons::ResultOf::GetState Buttons::SyncClient::GetState(::llcpp::fuchsia::buttons::ButtonType type) {
    return ResultOf::GetState(::zx::unowned_channel(this->channel_), std::move(type));
}

Buttons::ResultOf::GetState Buttons::Call::GetState(::zx::unowned_channel _client_end, ::llcpp::fuchsia::buttons::ButtonType type) {
  return ResultOf::GetState(std::move(_client_end), std::move(type));
}

template <>
Buttons::UnownedResultOf::GetState_Impl<Buttons::GetStateResponse>::GetState_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::llcpp::fuchsia::buttons::ButtonType type, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < GetStateRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<GetStateResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  memset(_request_buffer.data(), 0, GetStateRequest::PrimarySize);
  auto& _request = *reinterpret_cast<GetStateRequest*>(_request_buffer.data());
  _request.type = std::move(type);
  _request_buffer.set_actual(sizeof(GetStateRequest));
  ::fidl::DecodedMessage<GetStateRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      Buttons::InPlace::GetState(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Buttons::UnownedResultOf::GetState Buttons::SyncClient::GetState(::fidl::BytePart _request_buffer, ::llcpp::fuchsia::buttons::ButtonType type, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetState(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(type), std::move(_response_buffer));
}

Buttons::UnownedResultOf::GetState Buttons::Call::GetState(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::llcpp::fuchsia::buttons::ButtonType type, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetState(std::move(_client_end), std::move(_request_buffer), std::move(type), std::move(_response_buffer));
}

::fidl::DecodeResult<Buttons::GetStateResponse> Buttons::InPlace::GetState(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<GetStateRequest> params, ::fidl::BytePart response_buffer) {
  Buttons::SetTransactionHeaderFor::GetStateRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Buttons::GetStateResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<GetStateRequest, GetStateResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Buttons::GetStateResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
Buttons::ResultOf::RegisterNotify_Impl<Buttons::RegisterNotifyResponse>::RegisterNotify_Impl(::zx::unowned_channel _client_end, uint8_t types) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<RegisterNotifyRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, RegisterNotifyRequest::PrimarySize);
  auto& _request = *reinterpret_cast<RegisterNotifyRequest*>(_write_bytes);
  _request.types = std::move(types);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(RegisterNotifyRequest));
  ::fidl::DecodedMessage<RegisterNotifyRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      Buttons::InPlace::RegisterNotify(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Buttons::ResultOf::RegisterNotify Buttons::SyncClient::RegisterNotify(uint8_t types) {
    return ResultOf::RegisterNotify(::zx::unowned_channel(this->channel_), std::move(types));
}

Buttons::ResultOf::RegisterNotify Buttons::Call::RegisterNotify(::zx::unowned_channel _client_end, uint8_t types) {
  return ResultOf::RegisterNotify(std::move(_client_end), std::move(types));
}

template <>
Buttons::UnownedResultOf::RegisterNotify_Impl<Buttons::RegisterNotifyResponse>::RegisterNotify_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, uint8_t types, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < RegisterNotifyRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<RegisterNotifyResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  memset(_request_buffer.data(), 0, RegisterNotifyRequest::PrimarySize);
  auto& _request = *reinterpret_cast<RegisterNotifyRequest*>(_request_buffer.data());
  _request.types = std::move(types);
  _request_buffer.set_actual(sizeof(RegisterNotifyRequest));
  ::fidl::DecodedMessage<RegisterNotifyRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      Buttons::InPlace::RegisterNotify(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Buttons::UnownedResultOf::RegisterNotify Buttons::SyncClient::RegisterNotify(::fidl::BytePart _request_buffer, uint8_t types, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::RegisterNotify(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(types), std::move(_response_buffer));
}

Buttons::UnownedResultOf::RegisterNotify Buttons::Call::RegisterNotify(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, uint8_t types, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::RegisterNotify(std::move(_client_end), std::move(_request_buffer), std::move(types), std::move(_response_buffer));
}

::fidl::DecodeResult<Buttons::RegisterNotifyResponse> Buttons::InPlace::RegisterNotify(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<RegisterNotifyRequest> params, ::fidl::BytePart response_buffer) {
  Buttons::SetTransactionHeaderFor::RegisterNotifyRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Buttons::RegisterNotifyResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<RegisterNotifyRequest, RegisterNotifyResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Buttons::RegisterNotifyResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

zx_status_t Buttons::SyncClient::HandleEvents(Buttons::EventHandlers handlers) {
  return Buttons::Call::HandleEvents(::zx::unowned_channel(channel_), std::move(handlers));
}

zx_status_t Buttons::Call::HandleEvents(::zx::unowned_channel client_end, Buttons::EventHandlers handlers) {
  zx_status_t status = client_end->wait_one(ZX_CHANNEL_READABLE | ZX_CHANNEL_PEER_CLOSED,
                                            ::zx::time::infinite(),
                                            nullptr);
  if (status != ZX_OK) {
    return status;
  }
  constexpr uint32_t kReadAllocSize = ([]() constexpr {
    uint32_t x = 0;
    if (::fidl::internal::ClampedMessageSize<NotifyResponse, ::fidl::MessageDirection::kReceiving>() >= x) {
      x = ::fidl::internal::ClampedMessageSize<NotifyResponse, ::fidl::MessageDirection::kReceiving>();
    }
    return x;
  })();
  constexpr uint32_t kHandleAllocSize = ([]() constexpr {
    uint32_t x = 0;
    if (NotifyResponse::MaxNumHandles >= x) {
      x = NotifyResponse::MaxNumHandles;
    }
    if (x > ZX_CHANNEL_MAX_MSG_HANDLES) {
      x = ZX_CHANNEL_MAX_MSG_HANDLES;
    }
    return x;
  })();
  ::fidl::internal::ByteStorage<kReadAllocSize> read_storage;
  uint8_t* read_bytes = read_storage.buffer().data();
  zx_handle_t read_handles[kHandleAllocSize];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  status = client_end->read(ZX_CHANNEL_READ_MAY_DISCARD,
                            read_bytes, read_handles,
                            kReadAllocSize, kHandleAllocSize,
                            &actual_bytes, &actual_handles);
  if (status == ZX_ERR_BUFFER_TOO_SMALL) {
    // Message size is unexpectedly larger than calculated.
    // This can only be due to a newer version of the protocol defining a new event,
    // whose size exceeds the maximum of known events in the current protocol.
    return handlers.unknown();
  }
  if (status != ZX_OK) {
    return status;
  }
  if (actual_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(read_handles, actual_handles);
    return ZX_ERR_INVALID_ARGS;
  }
  auto msg = fidl_msg_t {
      .bytes = read_bytes,
      .handles = read_handles,
      .num_bytes = actual_bytes,
      .num_handles = actual_handles
  };
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg.bytes);
  status = fidl_validate_txn_header(hdr);
  if (status != ZX_OK) {
    return status;
  }
  switch (hdr->ordinal) {
    case kButtons_Notify_Ordinal:
    case kButtons_Notify_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<NotifyResponse>(&msg);
      if (result.status != ZX_OK) {
        return result.status;
      }
      auto message = result.message.message();
      return handlers.notify(std::move(message->type), std::move(message->pressed));
    }
    default:
      zx_handle_close_many(read_handles, actual_handles);
      return handlers.unknown();
  }
}

bool Buttons::TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  if (msg->num_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_INVALID_ARGS);
    return true;
  }
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg->bytes);
  zx_status_t status = fidl_validate_txn_header(hdr);
  if (status != ZX_OK) {
    txn->Close(status);
    return true;
  }
  switch (hdr->ordinal) {
    case kButtons_GetState_Ordinal:
    case kButtons_GetState_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<GetStateRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->GetState(std::move(message->type),
          Interface::GetStateCompleter::Sync(txn));
      return true;
    }
    case kButtons_RegisterNotify_Ordinal:
    case kButtons_RegisterNotify_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<RegisterNotifyRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->RegisterNotify(std::move(message->types),
          Interface::RegisterNotifyCompleter::Sync(txn));
      return true;
    }
    default: {
      return false;
    }
  }
}

bool Buttons::Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  bool found = TryDispatch(impl, msg, txn);
  if (!found) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_NOT_SUPPORTED);
  }
  return found;
}


void Buttons::Interface::GetStateCompleterBase::Reply(bool pressed) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetStateResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<GetStateResponse*>(_write_bytes);
  Buttons::SetTransactionHeaderFor::GetStateResponse(
      ::fidl::DecodedMessage<GetStateResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetStateResponse::PrimarySize,
              GetStateResponse::PrimarySize)));
  _response.pressed = std::move(pressed);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(GetStateResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<GetStateResponse>(std::move(_response_bytes)));
}

void Buttons::Interface::GetStateCompleterBase::Reply(::fidl::BytePart _buffer, bool pressed) {
  if (_buffer.capacity() < GetStateResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  auto& _response = *reinterpret_cast<GetStateResponse*>(_buffer.data());
  Buttons::SetTransactionHeaderFor::GetStateResponse(
      ::fidl::DecodedMessage<GetStateResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetStateResponse::PrimarySize,
              GetStateResponse::PrimarySize)));
  _response.pressed = std::move(pressed);
  _buffer.set_actual(sizeof(GetStateResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<GetStateResponse>(std::move(_buffer)));
}

void Buttons::Interface::GetStateCompleterBase::Reply(::fidl::DecodedMessage<GetStateResponse> params) {
  Buttons::SetTransactionHeaderFor::GetStateResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void Buttons::Interface::RegisterNotifyCompleterBase::Reply(::llcpp::fuchsia::buttons::Buttons_RegisterNotify_Result result) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<RegisterNotifyResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize];
  RegisterNotifyResponse _response = {};
  Buttons::SetTransactionHeaderFor::RegisterNotifyResponse(
      ::fidl::DecodedMessage<RegisterNotifyResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              RegisterNotifyResponse::PrimarySize,
              RegisterNotifyResponse::PrimarySize)));
  _response.result = std::move(result);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}
void Buttons::Interface::RegisterNotifyCompleterBase::ReplySuccess() {
  Buttons_RegisterNotify_Response response;

  Reply(Buttons_RegisterNotify_Result::WithResponse(&response));
}
void Buttons::Interface::RegisterNotifyCompleterBase::ReplyError(int32_t error) {
  Reply(Buttons_RegisterNotify_Result::WithErr(&error));
}

void Buttons::Interface::RegisterNotifyCompleterBase::Reply(::fidl::BytePart _buffer, ::llcpp::fuchsia::buttons::Buttons_RegisterNotify_Result result) {
  if (_buffer.capacity() < RegisterNotifyResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  RegisterNotifyResponse _response = {};
  Buttons::SetTransactionHeaderFor::RegisterNotifyResponse(
      ::fidl::DecodedMessage<RegisterNotifyResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              RegisterNotifyResponse::PrimarySize,
              RegisterNotifyResponse::PrimarySize)));
  _response.result = std::move(result);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}
void Buttons::Interface::RegisterNotifyCompleterBase::ReplySuccess(::fidl::BytePart _buffer) {
  Buttons_RegisterNotify_Response response;

  Reply(std::move(_buffer), Buttons_RegisterNotify_Result::WithResponse(&response));
}

void Buttons::Interface::RegisterNotifyCompleterBase::Reply(::fidl::DecodedMessage<RegisterNotifyResponse> params) {
  Buttons::SetTransactionHeaderFor::RegisterNotifyResponse(params);
  CompleterBase::SendReply(std::move(params));
}


zx_status_t Buttons::SendNotifyEvent(::zx::unowned_channel _chan, ::llcpp::fuchsia::buttons::ButtonType type, bool pressed) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<NotifyResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<NotifyResponse*>(_write_bytes);
  Buttons::SetTransactionHeaderFor::NotifyResponse(
      ::fidl::DecodedMessage<NotifyResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              NotifyResponse::PrimarySize,
              NotifyResponse::PrimarySize)));
  _response.type = std::move(type);
  _response.pressed = std::move(pressed);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(NotifyResponse));
  return ::fidl::Write(::zx::unowned_channel(_chan), ::fidl::DecodedMessage<NotifyResponse>(std::move(_response_bytes)));
}

zx_status_t Buttons::SendNotifyEvent(::zx::unowned_channel _chan, ::fidl::BytePart _buffer, ::llcpp::fuchsia::buttons::ButtonType type, bool pressed) {
  if (_buffer.capacity() < NotifyResponse::PrimarySize) {
    return ZX_ERR_BUFFER_TOO_SMALL;
  }
  auto& _response = *reinterpret_cast<NotifyResponse*>(_buffer.data());
  Buttons::SetTransactionHeaderFor::NotifyResponse(
      ::fidl::DecodedMessage<NotifyResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              NotifyResponse::PrimarySize,
              NotifyResponse::PrimarySize)));
  _response.type = std::move(type);
  _response.pressed = std::move(pressed);
  _buffer.set_actual(sizeof(NotifyResponse));
  return ::fidl::Write(::zx::unowned_channel(_chan), ::fidl::DecodedMessage<NotifyResponse>(std::move(_buffer)));
}

zx_status_t Buttons::SendNotifyEvent(::zx::unowned_channel _chan, ::fidl::DecodedMessage<NotifyResponse> params) {
  Buttons::SetTransactionHeaderFor::NotifyResponse(params);
  return ::fidl::Write(::zx::unowned_channel(_chan), std::move(params));
}



void Buttons::SetTransactionHeaderFor::GetStateRequest(const ::fidl::DecodedMessage<Buttons::GetStateRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kButtons_GetState_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void Buttons::SetTransactionHeaderFor::GetStateResponse(const ::fidl::DecodedMessage<Buttons::GetStateResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kButtons_GetState_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void Buttons::SetTransactionHeaderFor::RegisterNotifyRequest(const ::fidl::DecodedMessage<Buttons::RegisterNotifyRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kButtons_RegisterNotify_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void Buttons::SetTransactionHeaderFor::RegisterNotifyResponse(const ::fidl::DecodedMessage<Buttons::RegisterNotifyResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kButtons_RegisterNotify_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void Buttons::SetTransactionHeaderFor::NotifyResponse(const ::fidl::DecodedMessage<Buttons::NotifyResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kButtons_Notify_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

}  // namespace buttons
}  // namespace fuchsia
}  // namespace llcpp
