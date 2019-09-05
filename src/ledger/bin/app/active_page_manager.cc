// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/ledger/bin/app/active_page_manager.h"

#include <lib/callback/trace_callback.h>
#include <lib/fit/function.h>
#include <zircon/syscalls.h>

#include <algorithm>
#include <memory>
#include <stack>
#include <utility>
#include <vector>

#include "src/ledger/bin/app/page_utils.h"
#include "src/ledger/bin/fidl/include/types.h"
#include "src/ledger/bin/inspect/inspect.h"
#include "src/ledger/bin/storage/impl/data_serialization.h"
#include "src/lib/fxl/logging.h"

namespace ledger {
namespace {

// Precondition: ids_to_explore never empty.
// Precondition: known_commit_ids is always a superset of the IDs of the commits of
// explored_commits (IDs of garbage-collected commits will appear in known_commit_ids).
// Precondition: known_commit_ids is always a superset of ids_to_explore.
// Precondition: ids_to_explore is always disjoint with the IDs of the commits of explored_commits.
// TODO(https://bugs.fuchsia.dev/p/fuchsia/issues/detail?id=12338): Just call a method on
// PageStorage to afford all commits rather than traversing the graph.
void GatherCommits(
    std::stack<storage::CommitId> ids_to_explore,
    std::vector<std::unique_ptr<const storage::Commit>> explored_commits,
    std::set<storage::CommitId> known_commit_ids,
    fit::function<void(Status, std::vector<std::unique_ptr<const storage::Commit>>)> callback,
    storage::PageStorage* page_storage) {
  FXL_DCHECK(!ids_to_explore.empty());
  storage::CommitId id_to_explore = ids_to_explore.top();
  ids_to_explore.pop();
  page_storage->GetCommit(
      id_to_explore,
      [id_to_explore, ids_to_explore = std::move(ids_to_explore),
       explored_commits = std::move(explored_commits),
       known_commit_ids = std::move(known_commit_ids), callback = std::move(callback),
       page_storage](Status status, std::unique_ptr<const storage::Commit> commit) mutable {
        if (status == storage::Status::OK) {
          for (const storage::CommitIdView& parent_commit_id_view : commit->GetParentIds()) {
            storage::CommitId parent_commit_id = parent_commit_id_view.ToString();
            auto it = known_commit_ids.find(parent_commit_id);
            if (it == known_commit_ids.end()) {
              ids_to_explore.push(parent_commit_id);
              known_commit_ids.insert(parent_commit_id);
            }
          }
          explored_commits.push_back(std::move(commit));
        }
        // TODO(https://bugs.fuchsia.dev/p/fuchsia/issues/detail?id=35416): The status that
        // indicates commit-was-garbage-collected should not have "internal" in its name.
        else if (status == storage::Status::INTERNAL_NOT_FOUND) {
          // The commit may have existed once but was garbage-collected.
        } else {
          callback(status, {});
          return;
        }

        if (ids_to_explore.empty()) {
          callback(Status::OK, std::move(explored_commits));
        } else {
          GatherCommits(std::move(ids_to_explore), std::move(explored_commits),
                        std::move(known_commit_ids), std::move(callback), page_storage);
        }
      });
}

}  // namespace

ActivePageManager::ActivePageManager(Environment* environment,
                                     std::unique_ptr<storage::PageStorage> page_storage,
                                     std::unique_ptr<sync_coordinator::PageSync> page_sync,
                                     std::unique_ptr<MergeResolver> merge_resolver,
                                     ActivePageManager::PageStorageState state,
                                     zx::duration sync_timeout)
    : environment_(environment),
      page_storage_(std::move(page_storage)),
      page_sync_(std::move(page_sync)),
      merge_resolver_(std::move(merge_resolver)),
      sync_timeout_(sync_timeout),
      ongoing_page_storage_uses_(0),
      task_runner_(environment->dispatcher()) {
  page_delegates_.set_on_empty([this] { CheckEmpty(); });
  snapshots_.set_on_empty([this] { CheckEmpty(); });

  if (page_sync_) {
    page_sync_->SetSyncWatcher(&watchers_);
    page_sync_->SetOnIdle([this] { CheckEmpty(); });
    page_sync_->SetOnBacklogDownloaded([this] { OnSyncBacklogDownloaded(); });
    page_sync_->Start();
    if (state == ActivePageManager::PageStorageState::NEEDS_SYNC) {
      // The page storage was created locally. We wait a bit in order to get the
      // initial state from the network before accepting requests.
      task_runner_.PostDelayedTask(
          [this] {
            if (!sync_backlog_downloaded_) {
              FXL_LOG(INFO) << "Initial sync will continue in background, "
                            << "in the meantime binding to local page data "
                            << "(might be stale or empty).";
              OnSyncBacklogDownloaded();
            }
          },
          sync_timeout_);
    } else {
      sync_backlog_downloaded_ = true;
    }
  } else {
    sync_backlog_downloaded_ = true;
  }
  merge_resolver_->set_on_empty([this] { CheckEmpty(); });
  merge_resolver_->SetActivePageManager(this);
}

ActivePageManager::~ActivePageManager() {
  for (const auto& [page_impl, on_done] : page_impls_) {
    on_done(Status::INTERNAL_ERROR);
  }
  page_impls_.clear();
}

void ActivePageManager::AddPageImpl(std::unique_ptr<PageImpl> page_impl,
                                    fit::function<void(Status)> on_done) {
  auto traced_on_done = TRACE_CALLBACK(std::move(on_done), "ledger", "page_manager_add_page_impl");
  if (!sync_backlog_downloaded_) {
    page_impls_.emplace_back(std::move(page_impl), std::move(traced_on_done));
    return;
  }
  page_delegates_
      .emplace(environment_->coroutine_service(), this, page_storage_.get(), merge_resolver_.get(),
               &watchers_, std::move(page_impl))
      // Note that if the page connection is already cut at this point, |Init()|
      // will delete the newly created PageDelegate.
      .Init(std::move(traced_on_done));
}

void ActivePageManager::BindPageSnapshot(std::unique_ptr<const storage::Commit> commit,
                                         fidl::InterfaceRequest<PageSnapshot> snapshot_request,
                                         std::string key_prefix) {
  snapshots_.emplace(std::move(snapshot_request), page_storage_.get(), std::move(commit),
                     std::move(key_prefix));
}

Reference ActivePageManager::CreateReference(storage::ObjectIdentifier object_identifier) {
  uint64_t index = environment_->random()->Draw<uint64_t>();
  FXL_DCHECK(references_.find(index) == references_.end());
  references_[index] = std::move(object_identifier);
  Reference reference;
  reference.opaque_id = convert::ToArray(storage::SerializeData(index));
  return reference;
}

Status ActivePageManager::ResolveReference(Reference reference,
                                           storage::ObjectIdentifier* object_identifier) {
  if (reference.opaque_id.size() != sizeof(uint64_t)) {
    return Status::INVALID_ARGUMENT;
  }
  uint64_t index = storage::DeserializeData<uint64_t>(convert::ToStringView(reference.opaque_id));
  auto iterator = references_.find(index);
  if (iterator == references_.end()) {
    return Status::INVALID_ARGUMENT;
  }
  *object_identifier = iterator->second;
  return Status::OK;
}

void ActivePageManager::IsSynced(fit::function<void(Status, bool)> callback) {
  page_storage_->IsSynced([callback = std::move(callback)](Status status, bool is_synced) {
    callback(status, is_synced);
  });
}

void ActivePageManager::IsOfflineAndEmpty(fit::function<void(Status, bool)> callback) {
  if (page_storage_->IsOnline()) {
    callback(Status::OK, false);
    return;
  }
  // The page is offline. Check and return if it's also empty.
  page_storage_->IsEmpty([callback = std::move(callback)](Status status, bool is_empty) {
    callback(status, is_empty);
  });
}

storage::Status ActivePageManager::GetHeads(std::vector<const storage::CommitId>* heads) {
  std::vector<std::unique_ptr<const storage::Commit>> head_commits;
  storage::Status status = page_storage_->GetHeadCommits(&head_commits);
  if (status != storage::Status::OK) {
    return status;
  }
  heads->reserve(head_commits.size());
  for (const auto& head_commit : head_commits) {
    heads->push_back(head_commit->GetId());
  }
  return storage::Status::OK;
}

void ActivePageManager::GetCommits(
    fit::function<void(Status, std::vector<std::unique_ptr<const storage::Commit>>)> callback) {
  std::vector<std::unique_ptr<const storage::Commit>> head_commits;
  storage::Status status = page_storage_->GetHeadCommits(&head_commits);
  if (status != storage::Status::OK) {
    FXL_LOG(WARNING) << "GetHeadCommits returned non-OK status: " << status;
    callback(status, {});
    return;
  }
  std::set<storage::CommitId> known_commit_ids;
  std::stack<storage::CommitId> ids_to_explore;
  for (std::unique_ptr<const storage::Commit>& head_commit : head_commits) {
    known_commit_ids.insert(head_commit->GetId());
    for (const storage::CommitIdView& parent_commit_id_view : head_commit->GetParentIds()) {
      storage::CommitId parent_commit_id = parent_commit_id_view.ToString();
      ids_to_explore.push(parent_commit_id);
      known_commit_ids.insert(parent_commit_id);
    }
  }
  if (ids_to_explore.empty()) {
    callback(Status::OK, std::move(head_commits));
  } else {
    ongoing_page_storage_uses_++;
    GatherCommits(
        std::move(ids_to_explore), std::move(head_commits), std::move(known_commit_ids),
        [this, callback = std::move(callback)](
            Status status, std::vector<std::unique_ptr<const storage::Commit>> commits) {
          callback(status, std::move(commits));
          ongoing_page_storage_uses_--;
          CheckEmpty();
        },
        page_storage_.get());
  }
}

void ActivePageManager::GetCommit(
    const storage::CommitId& commit_id,
    fit::function<void(Status, std::unique_ptr<const storage::Commit>)> callback) {
  ongoing_page_storage_uses_++;
  page_storage_->GetCommit(
      commit_id, [this, callback = std::move(callback)](
                     storage::Status status, std::unique_ptr<const storage::Commit> commit) {
        callback(status, std::move(commit));
        ongoing_page_storage_uses_--;
        CheckEmpty();
      });
}

bool ActivePageManager::IsEmpty() {
  return page_delegates_.empty() && snapshots_.empty() && page_impls_.empty() &&
         merge_resolver_->IsEmpty() && (!page_sync_ || page_sync_->IsIdle()) &&
         ongoing_page_storage_uses_ == 0;
}

void ActivePageManager::CheckEmpty() {
  if (on_empty_callback_ && IsEmpty()) {
    on_empty_callback_();
  }
}

void ActivePageManager::OnSyncBacklogDownloaded() {
  sync_backlog_downloaded_ = true;
  for (auto& [page_impl, on_done] : page_impls_) {
    AddPageImpl(std::move(page_impl), std::move(on_done));
  }
  page_impls_.clear();
}

}  // namespace ledger
