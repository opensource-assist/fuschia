// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "garnet/lib/debug_ipc/client_protocol.h"

#include "garnet/lib/debug_ipc/message_reader.h"
#include "garnet/lib/debug_ipc/message_writer.h"
#include "garnet/lib/debug_ipc/protocol_helpers.h"

namespace debug_ipc {

// Record deserializers --------------------------------------------------------

bool Deserialize(MessageReader* reader, ProcessTreeRecord* record) {
  if (!reader->ReadUint32(reinterpret_cast<uint32_t*>(&record->type)))
    return false;
  if (!reader->ReadUint64(&record->koid)) return false;
  if (!reader->ReadString(&record->name)) return false;
  return Deserialize(reader, &record->children);
}

bool Deserialize(MessageReader* reader, ThreadRecord* record) {
  if (!reader->ReadUint64(&record->koid)) return false;
  if (!reader->ReadString(&record->name)) return false;

  uint32_t state;
  if (!reader->ReadUint32(&state)) return false;
  if (state >= static_cast<uint32_t>(ThreadRecord::State::kLast)) return false;
  record->state = static_cast<ThreadRecord::State>(state);
  return true;
}

bool Deserialize(MessageReader* reader, MemoryBlock* block) {
  if (!reader->ReadUint64(&block->address)) return false;

  uint32_t valid_flag;
  if (!reader->ReadUint32(&valid_flag)) return false;
  block->valid = !!valid_flag;

  if (!reader->ReadUint32(&block->size)) return false;
  if (block->valid) {
    if (block->size > reader->remaining()) return false;

    block->data.resize(block->size);
    if (!reader->ReadBytes(block->size, &block->data[0])) return false;
  }
  return true;
}

bool Deserialize(MessageReader* reader, Module* module) {
  if (!reader->ReadString(&module->name)) return false;
  if (!reader->ReadUint64(&module->base)) return false;
  if (!reader->ReadString(&module->build_id)) return false;
  return true;
};

bool Deserialize(MessageReader* reader, StackFrame* frame) {
  return reader->ReadBytes(sizeof(StackFrame), frame);
}

// Record serializers ----------------------------------------------------------

void Serialize(const ProcessBreakpointSettings& settings, MessageWriter* writer) {
  writer->WriteUint64(settings.process_koid);
  writer->WriteUint64(settings.thread_koid);
  writer->WriteUint64(settings.address);
}

void Serialize(const BreakpointSettings& settings, MessageWriter* writer) {
  writer->WriteUint32(settings.breakpoint_id);
  writer->WriteUint32(settings.one_shot ? 1 : 0);
  writer->WriteUint32(static_cast<uint32_t>(settings.stop));
  Serialize(settings.locations, writer);
}

// Hello -----------------------------------------------------------------------

void WriteRequest(const HelloRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kHello, transaction_id);
}

bool ReadReply(MessageReader* reader, HelloReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;
  return reader->ReadBytes(sizeof(HelloReply), reply);
}

// Launch ----------------------------------------------------------------------

void WriteRequest(const LaunchRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kLaunch, transaction_id);
  Serialize(request.argv, writer);
}

bool ReadReply(MessageReader* reader, LaunchReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  if (!reader->ReadUint32(&reply->status)) return false;
  if (!reader->ReadUint64(&reply->process_koid)) return false;
  if (!reader->ReadString(&reply->process_name)) return false;
  return true;
}

// Kill ----------------------------------------------------------------------

void WriteRequest(const KillRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kKill, transaction_id);
  writer->WriteUint64(request.process_koid);
}

bool ReadReply(MessageReader* reader, KillReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  if (!reader->ReadUint32(&reply->status)) return false;
  return true;
}

// Attach ----------------------------------------------------------------------

void WriteRequest(const AttachRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kAttach, transaction_id);
  writer->WriteUint64(request.koid);
}

bool ReadReply(MessageReader* reader, AttachReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  if (!reader->ReadUint32(&reply->status)) return false;
  if (!reader->ReadString(&reply->process_name)) return false;
  return true;
}

// Detach ----------------------------------------------------------------------

void WriteRequest(const DetachRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kDetach, transaction_id);
  writer->WriteUint64(request.process_koid);
}

bool ReadReply(MessageReader* reader, DetachReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  if (!reader->ReadUint32(&reply->status)) return false;
  return true;
}

// Pause -----------------------------------------------------------------------

void WriteRequest(const PauseRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kPause, transaction_id);
  writer->WriteUint64(request.process_koid);
  writer->WriteUint64(request.thread_koid);
}

bool ReadReply(MessageReader* reader, PauseReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;
  return true;
}

// Resume ----------------------------------------------------------------------

void WriteRequest(const ResumeRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kResume, transaction_id);
  writer->WriteUint64(request.process_koid);
  writer->WriteUint64(request.thread_koid);
  writer->WriteUint32(static_cast<uint32_t>(request.how));
}

bool ReadReply(MessageReader* reader, ResumeReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;
  return true;
}

// ProcessTree -----------------------------------------------------------------

void WriteRequest(const ProcessTreeRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kProcessTree, transaction_id);
  writer->WriteBytes(&request, sizeof(ProcessTreeRequest));
}

bool ReadReply(MessageReader* reader, ProcessTreeReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;
  return Deserialize(reader, &reply->root);
}

// Threads ---------------------------------------------------------------------

void WriteRequest(const ThreadsRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kThreads, transaction_id);
  writer->WriteBytes(&request, sizeof(ThreadsRequest));
}

bool ReadReply(MessageReader* reader, ThreadsReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  return Deserialize(reader, &reply->threads);
}

// ReadMemory ------------------------------------------------------------------

void WriteRequest(const ReadMemoryRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kReadMemory, transaction_id);
  writer->WriteBytes(&request, sizeof(ReadMemoryRequest));
}

bool ReadReply(MessageReader* reader, ReadMemoryReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  return Deserialize(reader, &reply->blocks);
}

// AddOrChangeBreakpoint -------------------------------------------------------

void WriteRequest(const AddOrChangeBreakpointRequest& request,
                  uint32_t transaction_id, MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kAddOrChangeBreakpoint, transaction_id);
  Serialize(request.breakpoint, writer);
}

bool ReadReply(MessageReader* reader, AddOrChangeBreakpointReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  return reader->ReadUint32(&reply->status);
}

// RemoveBreakpoint ------------------------------------------------------------

void WriteRequest(const RemoveBreakpointRequest& request,
                  uint32_t transaction_id, MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kRemoveBreakpoint, transaction_id);
  writer->WriteBytes(&request, sizeof(RemoveBreakpointRequest));
}

bool ReadReply(MessageReader* reader, RemoveBreakpointReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;
  return true;
}

// Backtrace -------------------------------------------------------------------

void WriteRequest(const BacktraceRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kBacktrace, transaction_id);
  writer->WriteBytes(&request, sizeof(BacktraceRequest));
}

bool ReadReply(MessageReader* reader, BacktraceReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  Deserialize(reader, &reply->frames);
  return true;
}

// Modules ---------------------------------------------------------------------

void WriteRequest(const ModulesRequest& request, uint32_t transaction_id,
                  MessageWriter* writer) {
  writer->WriteHeader(MsgHeader::Type::kModules, transaction_id);
  writer->WriteBytes(&request, sizeof(ModulesRequest));
}

bool ReadReply(MessageReader* reader, ModulesReply* reply,
               uint32_t* transaction_id) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  *transaction_id = header.transaction_id;

  Deserialize(reader, &reply->modules);
  return true;
}

// Notifications ---------------------------------------------------------------

bool ReadNotifyProcess(MessageReader* reader, NotifyProcess* process) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  if (!reader->ReadUint64(&process->process_koid)) return false;
  if (!reader->ReadInt64(&process->return_code)) return false;
  return true;
}

bool ReadNotifyThread(MessageReader* reader, NotifyThread* notify) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  if (!reader->ReadUint64(&notify->process_koid)) return false;
  return Deserialize(reader, &notify->record);
}

bool ReadNotifyException(MessageReader* reader, NotifyException* notify) {
  MsgHeader header;
  if (!reader->ReadHeader(&header)) return false;
  if (!reader->ReadUint64(&notify->process_koid)) return false;
  if (!Deserialize(reader, &notify->thread)) return false;

  uint32_t type;
  if (!reader->ReadUint32(&type) ||
      type >= static_cast<uint32_t>(NotifyException::Type::kLast))
    return false;
  notify->type = static_cast<NotifyException::Type>(type);

  return Deserialize(reader, &notify->frame);
}

}  // namespace debug_ipc
