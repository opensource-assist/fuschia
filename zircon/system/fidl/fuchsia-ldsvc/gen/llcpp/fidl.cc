// WARNING: This file is machine generated by fidlgen.

#include <fuchsia/ldsvc/llcpp/fidl.h>
#include <memory>

namespace llcpp {

namespace fuchsia {
namespace ldsvc {

namespace {

[[maybe_unused]]
constexpr uint64_t kLoader_Done_Ordinal = 0x501635da00000000lu;
[[maybe_unused]]
constexpr uint64_t kLoader_Done_GenOrdinal = 0x63ba6b76d3671001lu;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderDoneRequestTable;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderDoneResponseTable;
extern "C" const fidl_type_t v1_fuchsia_ldsvc_LoaderDoneResponseTable;
[[maybe_unused]]
constexpr uint64_t kLoader_LoadObject_Ordinal = 0x18d35e6000000000lu;
[[maybe_unused]]
constexpr uint64_t kLoader_LoadObject_GenOrdinal = 0x48c5a151d6df2853lu;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderLoadObjectRequestTable;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderLoadObjectResponseTable;
extern "C" const fidl_type_t v1_fuchsia_ldsvc_LoaderLoadObjectResponseTable;
[[maybe_unused]]
constexpr uint64_t kLoader_Config_Ordinal = 0x1adeb78d00000000lu;
[[maybe_unused]]
constexpr uint64_t kLoader_Config_GenOrdinal = 0x6a8a1a1464632841lu;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderConfigRequestTable;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderConfigResponseTable;
extern "C" const fidl_type_t v1_fuchsia_ldsvc_LoaderConfigResponseTable;
[[maybe_unused]]
constexpr uint64_t kLoader_Clone_Ordinal = 0x3862fcb900000000lu;
[[maybe_unused]]
constexpr uint64_t kLoader_Clone_GenOrdinal = 0x57e643a9ab6e4c29lu;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderCloneRequestTable;
extern "C" const fidl_type_t fuchsia_ldsvc_LoaderCloneResponseTable;
extern "C" const fidl_type_t v1_fuchsia_ldsvc_LoaderCloneResponseTable;

}  // namespace

Loader::ResultOf::Done_Impl::Done_Impl(::zx::unowned_channel _client_end) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<DoneRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, DoneRequest::PrimarySize);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(DoneRequest));
  ::fidl::DecodedMessage<DoneRequest> _decoded_request(std::move(_request_bytes));
  Super::operator=(
      Loader::InPlace::Done(std::move(_client_end)));
}

Loader::ResultOf::Done Loader::SyncClient::Done() {
    return ResultOf::Done(::zx::unowned_channel(this->channel_));
}

Loader::ResultOf::Done Loader::Call::Done(::zx::unowned_channel _client_end) {
  return ResultOf::Done(std::move(_client_end));
}

::fidl::internal::StatusAndError Loader::InPlace::Done(::zx::unowned_channel _client_end) {
  constexpr uint32_t _write_num_bytes = sizeof(DoneRequest);
  ::fidl::internal::AlignedBuffer<_write_num_bytes> _write_bytes;
  ::fidl::BytePart _request_buffer = _write_bytes.view();
  _request_buffer.set_actual(_write_num_bytes);
  ::fidl::DecodedMessage<DoneRequest> params(std::move(_request_buffer));
  Loader::SetTransactionHeaderFor::DoneRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::internal::StatusAndError::FromFailure(
        std::move(_encode_request_result));
  }
  zx_status_t _write_status =
      ::fidl::Write(std::move(_client_end), std::move(_encode_request_result.message));
  if (_write_status != ZX_OK) {
    return ::fidl::internal::StatusAndError(_write_status, ::fidl::internal::kErrorWriteFailed);
  } else {
    return ::fidl::internal::StatusAndError(ZX_OK, nullptr);
  }
}

template <>
Loader::ResultOf::LoadObject_Impl<Loader::LoadObjectResponse>::LoadObject_Impl(::zx::unowned_channel _client_end, ::fidl::StringView object_name) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<LoadObjectRequest, ::fidl::MessageDirection::kSending>();
  std::unique_ptr _write_bytes_boxed = std::make_unique<::fidl::internal::AlignedBuffer<_kWriteAllocSize>>();
  auto& _write_bytes_array = *_write_bytes_boxed;
  LoadObjectRequest _request = {};
  _request.object_name = std::move(object_name);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<LoadObjectRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Loader::InPlace::LoadObject(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Loader::ResultOf::LoadObject Loader::SyncClient::LoadObject(::fidl::StringView object_name) {
    return ResultOf::LoadObject(::zx::unowned_channel(this->channel_), std::move(object_name));
}

Loader::ResultOf::LoadObject Loader::Call::LoadObject(::zx::unowned_channel _client_end, ::fidl::StringView object_name) {
  return ResultOf::LoadObject(std::move(_client_end), std::move(object_name));
}

template <>
Loader::UnownedResultOf::LoadObject_Impl<Loader::LoadObjectResponse>::LoadObject_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::StringView object_name, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < LoadObjectRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<LoadObjectResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  LoadObjectRequest _request = {};
  _request.object_name = std::move(object_name);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<LoadObjectRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Loader::InPlace::LoadObject(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Loader::UnownedResultOf::LoadObject Loader::SyncClient::LoadObject(::fidl::BytePart _request_buffer, ::fidl::StringView object_name, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::LoadObject(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(object_name), std::move(_response_buffer));
}

Loader::UnownedResultOf::LoadObject Loader::Call::LoadObject(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::StringView object_name, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::LoadObject(std::move(_client_end), std::move(_request_buffer), std::move(object_name), std::move(_response_buffer));
}

::fidl::DecodeResult<Loader::LoadObjectResponse> Loader::InPlace::LoadObject(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<LoadObjectRequest> params, ::fidl::BytePart response_buffer) {
  Loader::SetTransactionHeaderFor::LoadObjectRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Loader::LoadObjectResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<LoadObjectRequest, LoadObjectResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Loader::LoadObjectResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
Loader::ResultOf::Config_Impl<Loader::ConfigResponse>::Config_Impl(::zx::unowned_channel _client_end, ::fidl::StringView config) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ConfigRequest, ::fidl::MessageDirection::kSending>();
  std::unique_ptr _write_bytes_boxed = std::make_unique<::fidl::internal::AlignedBuffer<_kWriteAllocSize>>();
  auto& _write_bytes_array = *_write_bytes_boxed;
  ConfigRequest _request = {};
  _request.config = std::move(config);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<ConfigRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Loader::InPlace::Config(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Loader::ResultOf::Config Loader::SyncClient::Config(::fidl::StringView config) {
    return ResultOf::Config(::zx::unowned_channel(this->channel_), std::move(config));
}

Loader::ResultOf::Config Loader::Call::Config(::zx::unowned_channel _client_end, ::fidl::StringView config) {
  return ResultOf::Config(std::move(_client_end), std::move(config));
}

template <>
Loader::UnownedResultOf::Config_Impl<Loader::ConfigResponse>::Config_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::StringView config, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < ConfigRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<ConfigResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  ConfigRequest _request = {};
  _request.config = std::move(config);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<ConfigRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Loader::InPlace::Config(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Loader::UnownedResultOf::Config Loader::SyncClient::Config(::fidl::BytePart _request_buffer, ::fidl::StringView config, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Config(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(config), std::move(_response_buffer));
}

Loader::UnownedResultOf::Config Loader::Call::Config(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::StringView config, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Config(std::move(_client_end), std::move(_request_buffer), std::move(config), std::move(_response_buffer));
}

::fidl::DecodeResult<Loader::ConfigResponse> Loader::InPlace::Config(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<ConfigRequest> params, ::fidl::BytePart response_buffer) {
  Loader::SetTransactionHeaderFor::ConfigRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Loader::ConfigResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<ConfigRequest, ConfigResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Loader::ConfigResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
Loader::ResultOf::Clone_Impl<Loader::CloneResponse>::Clone_Impl(::zx::unowned_channel _client_end, ::zx::channel loader) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<CloneRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, CloneRequest::PrimarySize);
  auto& _request = *reinterpret_cast<CloneRequest*>(_write_bytes);
  _request.loader = std::move(loader);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(CloneRequest));
  ::fidl::DecodedMessage<CloneRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      Loader::InPlace::Clone(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Loader::ResultOf::Clone Loader::SyncClient::Clone(::zx::channel loader) {
    return ResultOf::Clone(::zx::unowned_channel(this->channel_), std::move(loader));
}

Loader::ResultOf::Clone Loader::Call::Clone(::zx::unowned_channel _client_end, ::zx::channel loader) {
  return ResultOf::Clone(std::move(_client_end), std::move(loader));
}

template <>
Loader::UnownedResultOf::Clone_Impl<Loader::CloneResponse>::Clone_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::zx::channel loader, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < CloneRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<CloneResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  memset(_request_buffer.data(), 0, CloneRequest::PrimarySize);
  auto& _request = *reinterpret_cast<CloneRequest*>(_request_buffer.data());
  _request.loader = std::move(loader);
  _request_buffer.set_actual(sizeof(CloneRequest));
  ::fidl::DecodedMessage<CloneRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      Loader::InPlace::Clone(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Loader::UnownedResultOf::Clone Loader::SyncClient::Clone(::fidl::BytePart _request_buffer, ::zx::channel loader, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Clone(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(loader), std::move(_response_buffer));
}

Loader::UnownedResultOf::Clone Loader::Call::Clone(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::zx::channel loader, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Clone(std::move(_client_end), std::move(_request_buffer), std::move(loader), std::move(_response_buffer));
}

::fidl::DecodeResult<Loader::CloneResponse> Loader::InPlace::Clone(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<CloneRequest> params, ::fidl::BytePart response_buffer) {
  Loader::SetTransactionHeaderFor::CloneRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Loader::CloneResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<CloneRequest, CloneResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Loader::CloneResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}


bool Loader::TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
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
    case kLoader_Done_Ordinal:
    case kLoader_Done_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<DoneRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      impl->Done(
          Interface::DoneCompleter::Sync(txn));
      return true;
    }
    case kLoader_LoadObject_Ordinal:
    case kLoader_LoadObject_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<LoadObjectRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->LoadObject(std::move(message->object_name),
          Interface::LoadObjectCompleter::Sync(txn));
      return true;
    }
    case kLoader_Config_Ordinal:
    case kLoader_Config_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<ConfigRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->Config(std::move(message->config),
          Interface::ConfigCompleter::Sync(txn));
      return true;
    }
    case kLoader_Clone_Ordinal:
    case kLoader_Clone_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<CloneRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->Clone(std::move(message->loader),
          Interface::CloneCompleter::Sync(txn));
      return true;
    }
    default: {
      return false;
    }
  }
}

bool Loader::Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  bool found = TryDispatch(impl, msg, txn);
  if (!found) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_NOT_SUPPORTED);
  }
  return found;
}


void Loader::Interface::LoadObjectCompleterBase::Reply(int32_t rv, ::zx::vmo object) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<LoadObjectResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<LoadObjectResponse*>(_write_bytes);
  Loader::SetTransactionHeaderFor::LoadObjectResponse(
      ::fidl::DecodedMessage<LoadObjectResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              LoadObjectResponse::PrimarySize,
              LoadObjectResponse::PrimarySize)));
  _response.rv = std::move(rv);
  _response.object = std::move(object);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(LoadObjectResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<LoadObjectResponse>(std::move(_response_bytes)));
}

void Loader::Interface::LoadObjectCompleterBase::Reply(::fidl::BytePart _buffer, int32_t rv, ::zx::vmo object) {
  if (_buffer.capacity() < LoadObjectResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  auto& _response = *reinterpret_cast<LoadObjectResponse*>(_buffer.data());
  Loader::SetTransactionHeaderFor::LoadObjectResponse(
      ::fidl::DecodedMessage<LoadObjectResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              LoadObjectResponse::PrimarySize,
              LoadObjectResponse::PrimarySize)));
  _response.rv = std::move(rv);
  _response.object = std::move(object);
  _buffer.set_actual(sizeof(LoadObjectResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<LoadObjectResponse>(std::move(_buffer)));
}

void Loader::Interface::LoadObjectCompleterBase::Reply(::fidl::DecodedMessage<LoadObjectResponse> params) {
  Loader::SetTransactionHeaderFor::LoadObjectResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void Loader::Interface::ConfigCompleterBase::Reply(int32_t rv) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ConfigResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<ConfigResponse*>(_write_bytes);
  Loader::SetTransactionHeaderFor::ConfigResponse(
      ::fidl::DecodedMessage<ConfigResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              ConfigResponse::PrimarySize,
              ConfigResponse::PrimarySize)));
  _response.rv = std::move(rv);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(ConfigResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<ConfigResponse>(std::move(_response_bytes)));
}

void Loader::Interface::ConfigCompleterBase::Reply(::fidl::BytePart _buffer, int32_t rv) {
  if (_buffer.capacity() < ConfigResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  auto& _response = *reinterpret_cast<ConfigResponse*>(_buffer.data());
  Loader::SetTransactionHeaderFor::ConfigResponse(
      ::fidl::DecodedMessage<ConfigResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              ConfigResponse::PrimarySize,
              ConfigResponse::PrimarySize)));
  _response.rv = std::move(rv);
  _buffer.set_actual(sizeof(ConfigResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<ConfigResponse>(std::move(_buffer)));
}

void Loader::Interface::ConfigCompleterBase::Reply(::fidl::DecodedMessage<ConfigResponse> params) {
  Loader::SetTransactionHeaderFor::ConfigResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void Loader::Interface::CloneCompleterBase::Reply(int32_t rv) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<CloneResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<CloneResponse*>(_write_bytes);
  Loader::SetTransactionHeaderFor::CloneResponse(
      ::fidl::DecodedMessage<CloneResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              CloneResponse::PrimarySize,
              CloneResponse::PrimarySize)));
  _response.rv = std::move(rv);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(CloneResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<CloneResponse>(std::move(_response_bytes)));
}

void Loader::Interface::CloneCompleterBase::Reply(::fidl::BytePart _buffer, int32_t rv) {
  if (_buffer.capacity() < CloneResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  auto& _response = *reinterpret_cast<CloneResponse*>(_buffer.data());
  Loader::SetTransactionHeaderFor::CloneResponse(
      ::fidl::DecodedMessage<CloneResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              CloneResponse::PrimarySize,
              CloneResponse::PrimarySize)));
  _response.rv = std::move(rv);
  _buffer.set_actual(sizeof(CloneResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<CloneResponse>(std::move(_buffer)));
}

void Loader::Interface::CloneCompleterBase::Reply(::fidl::DecodedMessage<CloneResponse> params) {
  Loader::SetTransactionHeaderFor::CloneResponse(params);
  CompleterBase::SendReply(std::move(params));
}



void Loader::SetTransactionHeaderFor::DoneRequest(const ::fidl::DecodedMessage<Loader::DoneRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kLoader_Done_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void Loader::SetTransactionHeaderFor::LoadObjectRequest(const ::fidl::DecodedMessage<Loader::LoadObjectRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kLoader_LoadObject_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void Loader::SetTransactionHeaderFor::LoadObjectResponse(const ::fidl::DecodedMessage<Loader::LoadObjectResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kLoader_LoadObject_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void Loader::SetTransactionHeaderFor::ConfigRequest(const ::fidl::DecodedMessage<Loader::ConfigRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kLoader_Config_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void Loader::SetTransactionHeaderFor::ConfigResponse(const ::fidl::DecodedMessage<Loader::ConfigResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kLoader_Config_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void Loader::SetTransactionHeaderFor::CloneRequest(const ::fidl::DecodedMessage<Loader::CloneRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kLoader_Clone_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void Loader::SetTransactionHeaderFor::CloneResponse(const ::fidl::DecodedMessage<Loader::CloneResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kLoader_Clone_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

}  // namespace ldsvc
}  // namespace fuchsia
}  // namespace llcpp
