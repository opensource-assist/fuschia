// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/media/audio/audio_core/audio_admin.h"

#include <fcntl.h>
#include <lib/async/cpp/task.h>
#include <lib/async/default.h>
#include <lib/fidl/cpp/clone.h>

#include <trace/event.h>

#include "src/lib/syslog/cpp/logger.h"
#include "src/media/audio/audio_core/usage_gain_adjustment.h"

namespace media::audio {
namespace {

// TODO(35491): Remove when transitioned to xunion; xunions generate these functions.
fuchsia::media::Usage Usage(fuchsia::media::AudioRenderUsage u) {
  fuchsia::media::Usage usage;
  usage.set_render_usage(u);
  return usage;
}

fuchsia::media::Usage Usage(fuchsia::media::AudioCaptureUsage u) {
  fuchsia::media::Usage usage;
  usage.set_capture_usage(u);
  return usage;
}

}  // namespace

AudioAdmin::AudioAdmin(UsageGainAdjustment* gain_adjustment, async_dispatcher_t* fidl_dispatcher,
                       PolicyActionReporter* policy_action_reporter)
    : AudioAdmin(
          BehaviorGain{
              .none_gain_db = 0.0f,
              .duck_gain_db = -14.0f,
              .mute_gain_db = fuchsia::media::audio::MUTED_GAIN_DB,
          },
          gain_adjustment, policy_action_reporter, fidl_dispatcher) {}

AudioAdmin::AudioAdmin(BehaviorGain behavior_gain, UsageGainAdjustment* gain_adjustment,
                       PolicyActionReporter* policy_action_reporter,
                       async_dispatcher_t* fidl_dispatcher)
    : behavior_gain_(behavior_gain),
      gain_adjustment_(*gain_adjustment),
      policy_action_reporter_(*policy_action_reporter),
      fidl_dispatcher_(fidl_dispatcher) {
  FX_DCHECK(gain_adjustment);
  FX_DCHECK(policy_action_reporter);
  FX_DCHECK(fidl_dispatcher);
}

void AudioAdmin::SetInteraction(fuchsia::media::Usage active, fuchsia::media::Usage affected,
                                fuchsia::media::Behavior behavior) {
  async::PostTask(fidl_dispatcher_, [this, active = std::move(active),
                                     affected = std::move(affected), behavior = behavior]() {
    TRACE_DURATION("audio", "AudioAdmin::SetInteraction");
    std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
    if (active.Which() == fuchsia::media::Usage::Tag::kCaptureUsage &&
        affected.Which() == fuchsia::media::Usage::Tag::kCaptureUsage) {
      active_rules_.SetRule(active.capture_usage(), affected.capture_usage(), behavior);
    } else if (active.Which() == fuchsia::media::Usage::Tag::kCaptureUsage &&
               affected.Which() == fuchsia::media::Usage::Tag::kRenderUsage) {
      active_rules_.SetRule(active.capture_usage(), affected.render_usage(), behavior);

    } else if (active.Which() == fuchsia::media::Usage::Tag::kRenderUsage &&
               affected.Which() == fuchsia::media::Usage::Tag::kCaptureUsage) {
      active_rules_.SetRule(active.render_usage(), affected.capture_usage(), behavior);

    } else if (active.Which() == fuchsia::media::Usage::Tag::kRenderUsage &&
               affected.Which() == fuchsia::media::Usage::Tag::kRenderUsage) {
      active_rules_.SetRule(active.render_usage(), affected.render_usage(), behavior);
    }
  });
}

bool AudioAdmin::IsActive(fuchsia::media::AudioRenderUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::IsActive(Render)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  auto usage_index = fidl::ToUnderlying(usage);
  return active_streams_playback_[usage_index].size() > 0;
}

bool AudioAdmin::IsActive(fuchsia::media::AudioCaptureUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::IsActive(Capture)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  auto usage_index = fidl::ToUnderlying(usage);
  return active_streams_capture_[usage_index].size() > 0;
}

void AudioAdmin::SetUsageNone(fuchsia::media::AudioRenderUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::SetUsageNone(Render)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  gain_adjustment_.SetRenderUsageGainAdjustment(usage, behavior_gain_.none_gain_db);
  policy_action_reporter_.ReportPolicyAction(Usage(usage), fuchsia::media::Behavior::NONE);
}

void AudioAdmin::SetUsageNone(fuchsia::media::AudioCaptureUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::SetUsageNone(Capture)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  gain_adjustment_.SetCaptureUsageGainAdjustment(usage, behavior_gain_.none_gain_db);
  policy_action_reporter_.ReportPolicyAction(Usage(usage), fuchsia::media::Behavior::NONE);
}

void AudioAdmin::SetUsageMute(fuchsia::media::AudioRenderUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::SetUsageMute(Render)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  gain_adjustment_.SetRenderUsageGainAdjustment(usage, behavior_gain_.mute_gain_db);
  policy_action_reporter_.ReportPolicyAction(Usage(usage), fuchsia::media::Behavior::MUTE);
}

void AudioAdmin::SetUsageMute(fuchsia::media::AudioCaptureUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::SetUsageMute(Capture)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  gain_adjustment_.SetCaptureUsageGainAdjustment(usage, behavior_gain_.mute_gain_db);
  policy_action_reporter_.ReportPolicyAction(Usage(usage), fuchsia::media::Behavior::MUTE);
}

void AudioAdmin::SetUsageDuck(fuchsia::media::AudioRenderUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::SetUsageDuck(Render)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  gain_adjustment_.SetRenderUsageGainAdjustment(usage, behavior_gain_.duck_gain_db);
  policy_action_reporter_.ReportPolicyAction(Usage(usage), fuchsia::media::Behavior::DUCK);
}

void AudioAdmin::SetUsageDuck(fuchsia::media::AudioCaptureUsage usage) {
  TRACE_DURATION("audio", "AudioAdmin::SetUsageDuck(Capture)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  gain_adjustment_.SetCaptureUsageGainAdjustment(usage, behavior_gain_.duck_gain_db);
  policy_action_reporter_.ReportPolicyAction(Usage(usage), fuchsia::media::Behavior::DUCK);
}

void AudioAdmin::ApplyPolicies(fuchsia::media::AudioCaptureUsage active) {
  TRACE_DURATION("audio", "AudioAdmin::ApplyPolicies(Capture)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  for (int i = 0; i < fuchsia::media::RENDER_USAGE_COUNT; i++) {
    auto affected = static_cast<fuchsia::media::AudioRenderUsage>(i);
    switch (active_rules_.GetPolicy(active, affected)) {
      case fuchsia::media::Behavior::NONE:
        SetUsageNone(affected);
        break;
      case fuchsia::media::Behavior::DUCK:
        SetUsageDuck(affected);
        break;
      case fuchsia::media::Behavior::MUTE:
        SetUsageMute(affected);
        break;
    }
  }
  for (int i = 0; i < fuchsia::media::CAPTURE_USAGE_COUNT; i++) {
    auto affected = static_cast<fuchsia::media::AudioCaptureUsage>(i);
    switch (active_rules_.GetPolicy(active, affected)) {
      case fuchsia::media::Behavior::NONE:
        SetUsageNone(affected);
        break;
      case fuchsia::media::Behavior::DUCK:
        SetUsageDuck(affected);
        break;
      case fuchsia::media::Behavior::MUTE:
        SetUsageMute(affected);
        break;
    }
  }
}

void AudioAdmin::ApplyPolicies(fuchsia::media::AudioRenderUsage active) {
  TRACE_DURATION("audio", "AudioAdmin::ApplyPolicies(Render)");
  std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
  for (int i = 0; i < fuchsia::media::RENDER_USAGE_COUNT; i++) {
    auto affected = static_cast<fuchsia::media::AudioRenderUsage>(i);
    switch (active_rules_.GetPolicy(active, affected)) {
      case fuchsia::media::Behavior::NONE:
        SetUsageNone(affected);
        break;
      case fuchsia::media::Behavior::DUCK:
        SetUsageDuck(affected);
        break;
      case fuchsia::media::Behavior::MUTE:
        SetUsageMute(affected);
        break;
    }
  }
  for (int i = 0; i < fuchsia::media::CAPTURE_USAGE_COUNT; i++) {
    auto affected = static_cast<fuchsia::media::AudioCaptureUsage>(i);
    switch (active_rules_.GetPolicy(active, affected)) {
      case fuchsia::media::Behavior::NONE:
        SetUsageNone(affected);
        break;
      case fuchsia::media::Behavior::DUCK:
        SetUsageDuck(affected);
        break;
      case fuchsia::media::Behavior::MUTE:
        SetUsageMute(affected);
        break;
    }
  }
}

void AudioAdmin::UpdatePolicy() {
  TRACE_DURATION("audio", "AudioAdmin::UpdatePolicy");
  // TODO(perley): convert this to an array of Usage unions or something else
  //               that makes it at least a little flexible.
  // The processing order of this represents the 'priorities' of the streams
  // with this implementation.
  if (IsActive(fuchsia::media::AudioCaptureUsage::COMMUNICATION)) {
    ApplyPolicies(fuchsia::media::AudioCaptureUsage::COMMUNICATION);
  } else if (IsActive(fuchsia::media::AudioRenderUsage::COMMUNICATION)) {
    ApplyPolicies(fuchsia::media::AudioRenderUsage::COMMUNICATION);
  } else if (IsActive(fuchsia::media::AudioCaptureUsage::SYSTEM_AGENT)) {
    ApplyPolicies(fuchsia::media::AudioCaptureUsage::SYSTEM_AGENT);
  } else if (IsActive(fuchsia::media::AudioRenderUsage::SYSTEM_AGENT)) {
    ApplyPolicies(fuchsia::media::AudioRenderUsage::SYSTEM_AGENT);
  } else if (IsActive(fuchsia::media::AudioRenderUsage::INTERRUPTION)) {
    ApplyPolicies(fuchsia::media::AudioRenderUsage::INTERRUPTION);
  } else if (IsActive(fuchsia::media::AudioCaptureUsage::FOREGROUND)) {
    ApplyPolicies(fuchsia::media::AudioCaptureUsage::FOREGROUND);
  } else if (IsActive(fuchsia::media::AudioRenderUsage::MEDIA)) {
    ApplyPolicies(fuchsia::media::AudioRenderUsage::MEDIA);
  } else if (IsActive(fuchsia::media::AudioCaptureUsage::BACKGROUND)) {
    ApplyPolicies(fuchsia::media::AudioCaptureUsage::BACKGROUND);
  } else {
    ApplyPolicies(fuchsia::media::AudioRenderUsage::BACKGROUND);
  }
}

void AudioAdmin::UpdateRendererState(fuchsia::media::AudioRenderUsage usage, bool active,
                                     fuchsia::media::AudioRenderer* renderer) {
  async::PostTask(fidl_dispatcher_, [this, usage = usage, active = active, renderer = renderer] {
    TRACE_DURATION("audio", "AudioAdmin::UpdateRendererState");
    std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
    auto usage_index = fidl::ToUnderlying(usage);
    FX_DCHECK(usage_index < fuchsia::media::RENDER_USAGE_COUNT);
    if (active) {
      active_streams_playback_[usage_index].insert(renderer);
    } else {
      active_streams_playback_[usage_index].erase(renderer);
    }

    UpdatePolicy();
  });
}

void AudioAdmin::UpdateCapturerState(fuchsia::media::AudioCaptureUsage usage, bool active,
                                     fuchsia::media::AudioCapturer* capturer) {
  async::PostTask(fidl_dispatcher_, [this, usage = usage, active = active, capturer = capturer] {
    TRACE_DURATION("audio", "AudioAdmin::UpdateCapturerState");
    std::lock_guard<fxl::ThreadChecker> lock(fidl_thread_checker_);
    auto usage_index = fidl::ToUnderlying(usage);
    FX_DCHECK(usage_index < fuchsia::media::CAPTURE_USAGE_COUNT);
    if (active) {
      active_streams_capture_[usage_index].insert(capturer);
    } else {
      active_streams_capture_[usage_index].erase(capturer);
    }

    UpdatePolicy();
  });
}

void AudioAdmin::PolicyRules::ResetInteractions() {
  TRACE_DURATION("audio", "AudioAdmin::ResetInteractions");
  for (int i = 0; i < fuchsia::media::RENDER_USAGE_COUNT; i++) {
    auto active = static_cast<fuchsia::media::AudioRenderUsage>(i);
    for (int j = 0; j < fuchsia::media::RENDER_USAGE_COUNT; j++) {
      auto affected = static_cast<fuchsia::media::AudioRenderUsage>(j);
      SetRule(active, affected, fuchsia::media::Behavior::NONE);
    }
    for (int j = 0; j < fuchsia::media::CAPTURE_USAGE_COUNT + 1; j++) {
      auto affected = static_cast<fuchsia::media::AudioCaptureUsage>(j);
      SetRule(active, affected, fuchsia::media::Behavior::NONE);
    }
  }
  for (int i = 0; i < fuchsia::media::CAPTURE_USAGE_COUNT; i++) {
    auto active = static_cast<fuchsia::media::AudioCaptureUsage>(i);
    for (int j = 0; j < fuchsia::media::RENDER_USAGE_COUNT; j++) {
      auto affected = static_cast<fuchsia::media::AudioRenderUsage>(j);
      SetRule(active, affected, fuchsia::media::Behavior::NONE);
    }
    for (int j = 0; j < fuchsia::media::CAPTURE_USAGE_COUNT; j++) {
      auto affected = static_cast<fuchsia::media::AudioCaptureUsage>(j);
      SetRule(active, affected, fuchsia::media::Behavior::NONE);
    }
  }
}

void AudioAdmin::SetInteractionsFromAudioPolicy(AudioPolicy policy) {
  async::PostTask(fidl_dispatcher_, [this, policy = std::move(policy)] {
    ResetInteractions();
    for (auto& rule : policy.rules()) {
      SetInteraction(fidl::Clone(rule.active), fidl::Clone(rule.affected), rule.behavior);
    }
  });
}

}  // namespace media::audio
