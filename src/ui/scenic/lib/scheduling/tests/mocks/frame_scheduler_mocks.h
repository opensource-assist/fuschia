// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_UI_SCENIC_LIB_SCHEDULING_TESTS_MOCKS_FRAME_SCHEDULER_MOCKS_H_
#define SRC_UI_SCENIC_LIB_SCHEDULING_TESTS_MOCKS_FRAME_SCHEDULER_MOCKS_H_

#include <set>
#include <unordered_map>
#include <variant>

#include "src/ui/scenic/lib/scheduling/frame_scheduler.h"
#include "src/ui/scenic/lib/scheduling/frame_timings.h"
#include "src/ui/scenic/lib/scheduling/vsync_timing.h"

namespace scheduling {
namespace test {

class MockFrameScheduler : public FrameScheduler {
 public:
  MockFrameScheduler() = default;

  // |FrameScheduler|
  void SetFrameRenderer(fxl::WeakPtr<FrameRenderer> frame_renderer) override {}
  // |FrameScheduler|
  void AddSessionUpdater(fxl::WeakPtr<SessionUpdater> session_updater) override {}
  // |FrameScheduler|
  void SetRenderContinuously(bool render_continuously) override;

  // |FrameScheduler|
  void SetOnUpdateFailedCallbackForSession(
      SessionId session, OnSessionUpdateFailedCallback update_failed_callback) override {}

  // |FrameScheduler|
  void ScheduleUpdateForSession(zx::time presentation_time, SessionId session) override;

  // |FrameScheduler|
  void GetFuturePresentationInfos(
      zx::duration requested_prediction_span,
      FrameScheduler::GetFuturePresentationInfosCallback presentation_infos_callback) override;

  // |FrameScheduler|
  void SetOnFramePresentedCallbackForSession(
      SessionId session, OnFramePresentedCallback frame_presented_callback) override;

  // |FrameScheduler|
  void ClearCallbacksForSession(SessionId session_id) override {}

  // Testing only. Used for mock method callbacks.
  using OnSetRenderContinuouslyCallback = std::function<void(bool)>;
  using OnScheduleUpdateForSessionCallback = std::function<void(zx::time, SessionId)>;
  using OnGetFuturePresentationInfosCallback =
      std::function<std::vector<fuchsia::scenic::scheduling::PresentationInfo>(
          zx::duration requested_prediction_span)>;
  using OnSetOnFramePresentedCallbackForSessionCallback =
      std::function<void(SessionId session, OnFramePresentedCallback callback)>;

  // Testing only. Sets mock method callback.
  void set_set_render_continuously_callback(OnSetRenderContinuouslyCallback callback) {
    set_render_continuously_callback_ = callback;
  }

  // Testing only. Sets mock method callback.
  void set_schedule_update_for_session_callback(OnScheduleUpdateForSessionCallback callback) {
    schedule_update_for_session_callback_ = callback;
  }

  // Testing only. Sets mock method callback.
  void set_get_future_presentation_infos_callback(OnGetFuturePresentationInfosCallback callback) {
    get_future_presentation_infos_callback_ = callback;
  }

  // Testing only. Sets mock method callback.
  void set_set_on_frame_presented_callback_for_session_callback(
      OnSetOnFramePresentedCallbackForSessionCallback callback) {
    set_on_frame_presented_callback_for_session_callback_ = callback;
  }

 private:
  // Testing only. Mock method callbacks.
  OnSetRenderContinuouslyCallback set_render_continuously_callback_;
  OnScheduleUpdateForSessionCallback schedule_update_for_session_callback_;
  OnGetFuturePresentationInfosCallback get_future_presentation_infos_callback_;
  OnSetOnFramePresentedCallbackForSessionCallback
      set_on_frame_presented_callback_for_session_callback_;
};

class MockSessionUpdater : public SessionUpdater {
 public:
  MockSessionUpdater() : weak_factory_(this) {}

  // |SessionUpdater|
  SessionUpdater::UpdateResults UpdateSessions(std::unordered_set<SessionId> sessions_to_update,
                                               zx::time target_presentation_time,
                                               zx::time latched_time,
                                               uint64_t trace_id = 0) override;

  // |SessionUpdater|
  void PrepareFrame(zx::time presentation_time, uint64_t frame_number) override {
    ++prepare_frame_call_count_;
  }

  // AddCallback() adds a closure to be returned by UpdateSessions(), and returns a
  // CallbackStatus struct that can be used to observe the current status of the callback.
  struct CallbackStatus {
    // The SessionId that this update corresponds to.
    SessionId session_id = 0;
    // Number of times that the update was rescheduled due to the fences not
    // being ready.
    size_t reschedule_count = 0;
    // Becomes true when the associated callback is passed to the UpdateManager, i.e. after the
    // fences are reached and the update has been "applied" before "rendering".
    bool callback_passed = false;
    // Becomes true when the associated callback is invoked (the callback itself is created within
    // ScheduleUpdate(), and is not visible to the caller).
    bool callback_invoked = false;
    // Becomes true when the updater disappears before the callback is invoked.
    bool updater_disappeared = false;
    // The PresentationInfo that was passed to the callback, valid only if |callback_invoked| is
    // true.
    fuchsia::images::PresentationInfo presentation_info;
  };
  std::shared_ptr<const CallbackStatus> AddCallback(SessionId id, zx::time presentation_time,
                                                    zx::time acquire_fence_time);

  void AddPresent2Info(scheduling::Present2Info info, zx::time presentation_time,
                       zx::time acquire_fence_time);

  // By default, rendering is enabled and |UpdateSessions()| will return ".needs_render = true" if
  // any session updates were applied.  This allows a test to override that behavior to
  // unconditionally disable rendering.
  void SuppressNeedsRendering(bool should_suppress) { rendering_suppressed_ = should_suppress; }

  // By default, we expect that the sessions identified by UpdateSessions()'s |sessions_to_update|
  // will all have at least one update queued.  This will not be the case in multi-updater scenarios
  // (because each updater is responsible for only some of the sessions, and will therefore receive
  // unknown session IDs); this method relaxes the restriction for those tests.
  void BeRelaxedAboutUnexpectedSessionUpdates() {
    be_relaxed_about_unexpected_session_updates_ = true;
  }

  // Treats session_id as a failed update the next time UpdateSessions is called. This does not
  // persist for future updates.
  void SetNextUpdateForSessionFails(SessionId session_id) {
    sessions_to_fail_on_next_update_.insert(session_id);
  }

  // Simulate killing of a session.  This simply treats the session (and any associated updates) as
  // absent during |UpdateSession()|.
  void KillSession(SessionId session_id) { dead_sessions_.insert(session_id); }

  uint32_t update_sessions_call_count() { return update_sessions_call_count_; }
  uint32_t prepare_frame_call_count() { return prepare_frame_call_count_; }
  uint32_t signal_successful_present_callback_count() {
    return signal_successful_present_callback_count_;
  }

  fxl::WeakPtr<MockSessionUpdater> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }

 private:
  uint32_t update_sessions_call_count_ = 0;
  uint32_t prepare_frame_call_count_ = 0;
  uint32_t signal_successful_present_callback_count_ = 0;

  // Instances are generated by |AddCallback()|, and model the queuing of batched session
  // updates, and the callback that is invoked once the update has been applied to the
  // scene, and the corresponding frame rendered.
  struct Update {
    // Target presentation time.
    zx::time target;
    // Time that the fences will be finished.
    zx::time fences_done;
    // Updated to allow the test to track progress.
    std::shared_ptr<CallbackStatus> status;
    // Callback that will be invoked when UpdateManager::SignalPresentCallbacks() is called.
    OnPresentedCallback callback;
  };

  struct Present2Update {
    zx::time target_presentation_time;
    zx::time fences_done_time;

    scheduling::Present2Info present2_info;
  };

  std::map<SessionId, std::queue<Update>> updates_;
  std::map<SessionId, std::queue<Present2Update>> present2_updates_;

  std::unordered_set<SessionId> sessions_to_fail_on_next_update_;

  // Stores session IDs that were passed to |UpdateSessions()|, but for which no corresponding
  // updates were registered.
  std::set<SessionId> dead_sessions_;
  bool be_relaxed_about_unexpected_session_updates_ = false;

  // See |SuppressNeedsRendering()|.
  bool rendering_suppressed_ = false;

  fxl::WeakPtrFactory<MockSessionUpdater> weak_factory_;  // must be last
};

class MockFrameRenderer : public FrameRenderer {
 public:
  MockFrameRenderer() : weak_factory_(this) {}

  // |FrameRenderer|
  RenderFrameResult RenderFrame(fxl::WeakPtr<FrameTimings> frame_timings,
                                zx::time presentation_time) override;

  // Need to call this in order to trigger the OnFramePresented() callback in
  // FrameScheduler, but is not valid to do until after RenderFrame has returned
  // to FrameScheduler. Hence this separate method.
  void EndFrame(size_t frame_index, zx::time time_done);

  // Signal frame |frame_index| that it has been rendered.
  void SignalFrameRendered(uint64_t frame_number, zx::time time_done);

  // Signal frame |frame_index| that the CPU portion of rendering is done.
  void SignalFrameCpuRendered(uint64_t frame_number, zx::time time_done);

  // Signal frame |frame_index| that it has been presented.
  void SignalFramePresented(uint64_t frame_number, zx::time time_done);

  // Signal frame |frame_index| that it has been dropped.
  void SignalFrameDropped(uint64_t frame_number);

  // Manually set value returned from RenderFrame.
  void SetRenderFrameReturnValue(RenderFrameResult new_value) {
    render_frame_return_value_ = new_value;
  }
  uint32_t render_frame_call_count() { return render_frame_call_count_; }
  size_t pending_frames() { return frames_.size(); }

  fxl::WeakPtr<MockFrameRenderer> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }

 private:
  void CleanUpFrame(uint64_t frame_number);

  RenderFrameResult render_frame_return_value_ = RenderFrameResult::kRenderSuccess;
  uint32_t render_frame_call_count_ = 0;

  struct Timings {
    fxl::WeakPtr<FrameTimings> frame_timings;
    size_t swapchain_index = -1;
    uint32_t frame_rendered = false;
    uint32_t frame_cpu_rendered = false;
    uint32_t frame_presented = false;
  };
  std::unordered_map<uint64_t, Timings> frames_;

  uint64_t last_frame_number_ = -1;

  fxl::WeakPtrFactory<MockFrameRenderer> weak_factory_;  // must be last
};

}  // namespace test
}  // namespace scheduling

#endif  // SRC_UI_SCENIC_LIB_SCHEDULING_TESTS_MOCKS_FRAME_SCHEDULER_MOCKS_H_
