#ifndef PITCH_SHIFT_API_H_
#define PITCH_SHIFT_API_H_

#include <cstddef>
#include <memory>
#include <vector>

class Vocoder;
class Resampler;

class PitchShifter {
 public:
  using SizeType = std::size_t;

  PitchShifter(SizeType num_channels, SizeType num_samples,
               SizeType sample_rate);

  std::vector<std::vector<float>> ScaleTime(
      const std::vector<std::vector<float>>& src, float scale_factor);
  std::vector<std::vector<float>> ScalePitch(
      const std::vector<std::vector<float>>& src, float scale_factor);

 private:
  const SizeType kNumChannels;
  const SizeType kNumSamples;
  const SizeType kSampleRate;
  std::unique_ptr<Vocoder> vocoder_;
  std::unique_ptr<Resampler> resampler_;
};

#endif  // PITCH_SHIFT_API_H_