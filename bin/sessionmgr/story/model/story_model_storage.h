// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PERIDOT_BIN_SESSIONMGR_STORY_MODEL_STORY_MODEL_STORAGE_H_
#define PERIDOT_BIN_SESSIONMGR_STORY_MODEL_STORY_MODEL_STORAGE_H_

#include <fuchsia/modular/storymodel/cpp/fidl.h>
#include <lib/async_promise/executor.h>
#include <lib/fit/function.h>
#include <lib/fxl/macros.h>

#include <list>
#include <memory>

namespace modular {

// The purpose of a StoryModelStorage is to apply StoryModelMutations to a
// persistent storage layer and flow observed changes from the storage layer to
// Observe().
//
// Due to the nature of some storage systems (such as Ledger), a call to
// Mutate() doesn't necessarily result in an equivalent Observe().
//
// A trivial example: two peers, A and B, both call Mutate() with conflicting
// instructions. Peer A learns of peer B's mutation before calling Observe(),
// and conflict resolution results in no change to the model. In this case,
// Observe() will not be called.
class StoryModelStorage {
 public:
  StoryModelStorage();
  virtual ~StoryModelStorage();

  // Registers a callback that is called when Observe() is called by
  // implementers.
  void SetObserveCallback(
      fit::function<
          void(std::vector<fuchsia::modular::storymodel::StoryModelMutation>)>
          callback);

  virtual fit::promise<> Execute(
      std::vector<fuchsia::modular::storymodel::StoryModelMutation>
          commands) = 0;

 protected:
  // Calls to Observe() must always be made from the same thread: ordering
  // of the observed mutations matters.
  void Observe(std::vector<fuchsia::modular::storymodel::StoryModelMutation>);

 private:
  fit::function<void(
      std::vector<fuchsia::modular::storymodel::StoryModelMutation>)>
      observe_callback_;

  FXL_DISALLOW_COPY_AND_ASSIGN(StoryModelStorage);
};

}  // namespace modular

#endif  // PERIDOT_BIN_SESSIONMGR_STORY_MODEL_STORY_MODEL_STORAGE_H_
