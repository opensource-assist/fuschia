// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_LEDGER_LIB_VMO_SHARED_VMO_H_
#define SRC_LEDGER_LIB_VMO_SHARED_VMO_H_

#include <lib/zx/vmo.h>

#include <mutex>

#include "src/ledger/lib/memory/ref_counted.h"

namespace ledger {

// Holds a reference to a shared VMO which may be memory mapped lazily.
// Once memory-mapped, the VMO remains mapped until all references to this
// object have been released.
//
// This object is thread-safe.
class SharedVmo : public RefCountedThreadSafe<SharedVmo> {
 public:
  // Initializes a shared VMO.
  //
  // |vmo| must be a valid VMO handle.
  // If not zero, |map_flags| specifies the flags which should be passed to
  // |zx::vmar::map| when the VMO is mapped.
  explicit SharedVmo(zx::vmo vmo, uint32_t map_flags = 0u);
  SharedVmo(const SharedVmo&) = delete;
  SharedVmo& operator=(const SharedVmo&) = delete;
  virtual ~SharedVmo();

  // Gets the underlying VMO.
  const zx::vmo& vmo() const { return vmo_; }

  // Gets the size of the VMO.
  size_t vmo_size() const { return vmo_size_; }

  // Gets the flags used for mapping the VMO.
  uint32_t map_flags() const { return map_flags_; }

  // Maps the entire VMO into memory (if not already mapped).
  // Returns the address of the mapping or nullptr if an error occurred.
  void* Map();

 private:
  zx::vmo const vmo_;
  uint32_t const map_flags_;
  size_t vmo_size_;

  std::once_flag mapping_once_flag_{};
  uintptr_t mapping_ = 0u;

  LEDGER_FRIEND_REF_COUNTED_THREAD_SAFE(SharedVmo);
};

}  // namespace ledger

#endif  // SRC_LEDGER_LIB_VMO_SHARED_VMO_H_
