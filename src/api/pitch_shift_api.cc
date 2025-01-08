#include "pitch_shift_api.h"

#include <memory>
#include <vector>

PitchShifter::PitchShifter(const SizeType num_channels,
                           const SizeType num_samples,
                           const SizeType sample_rate)
    : kNumChannels(num_channels),
      kNumSamples(num_samples),
      kSampleRate(sample_rate),
      vocoder_(num_channels, num_samples, sample_rate) {
  // Initialize resampler.
}

std::vector<std::vector<float>> PitchShifter::ScaleTime(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  // TODO
  return std::vector<std::vector<float>>();
}

std::vector<std::vector<float>> PitchShifter::ScalePitch(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  // TODO
  return std::vector<std::vector<float>>();
}