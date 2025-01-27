#include "pitch_shift_api.h"

#include <memory>
#include <vector>

#include "resampler.h"
#include "vocoder.h"

PitchShifter::PitchShifter(const SizeType num_channels,
                           const SizeType num_samples,
                           const SizeType sample_rate)
    : kNumChannels(num_channels),
      kNumSamples(num_samples),
      kSampleRate(sample_rate),
      vocoder_(
          std::make_unique<Vocoder>(num_channels, num_samples, sample_rate)),
      resampler_(
          std::make_unique<Resampler>(48e3, 48e3, num_channels, num_samples)) {}

std::vector<std::vector<float>> PitchShifter::ScaleTime(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  return vocoder_->Process(src, Vocoder::kTimeStretch, scale_factor);
}

std::vector<std::vector<float>> PitchShifter::ScalePitch(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  resampler_->SetRate(kSampleRate * scale_factor);
  return resampler_->Resample(
      vocoder_->Process(src, Vocoder::kNone, scale_factor));
}