#include "pitch_shift_api.h"

#include <memory>
#include <vector>

#include "resampler.h"
#include "vocoder.h"

PitchShifter::PitchShifter(const SizeType num_channels,
                           const SizeType sample_rate)
    : kNumChannels(num_channels),
      kSampleRate(sample_rate),
      vocoder_(
          std::make_unique<Vocoder>(num_channels, 4 * kFrameSize, sample_rate)),
      resampler_(std::make_unique<Resampler>(48e3, 48e3, num_channels,
                                             4 * kFrameSize)) {
  input_buffer_.resize(num_channels, std::vector<float>(4 * kFrameSize, 0.f));
}

std::vector<std::vector<float>> PitchShifter::ScaleTime(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  // During development let's only allow a frame size of 256.
  if (kFrameSize != 256) {
    throw std::runtime_error("Debug: Frame size must be 256 for now.");
  }

  // Initially we start with an empty buffer of size 4 * frame_size.
  // Each time we receive a new frame, we left shift the buffer by frame_size
  // and append the new frame at the end of the buffer.
  for (SizeType i = 0; i < kNumChannels; ++i) {
    std::copy(input_buffer_[i].begin() + kFrameSize, input_buffer_[i].end(),
              input_buffer_[i].begin());
    std::copy(src[i].begin(), src[i].end(),
              input_buffer_[i].begin() + 3 * kFrameSize);
  }
  return vocoder_->Process(input_buffer_, Vocoder::kTimeStretch, scale_factor);
}

std::vector<std::vector<float>> PitchShifter::ScalePitch(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  resampler_->SetRate(kSampleRate * scale_factor);
  return resampler_->Resample(
      vocoder_->Process(src, Vocoder::kNone, scale_factor));
}