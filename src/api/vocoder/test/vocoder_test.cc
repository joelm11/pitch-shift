#include "../vocoder.h"

#include <gtest/gtest.h>

TEST(VocoderTest, Constructor) {
  const Vocoder::SizeType num_channels = 2;
  const Vocoder::SizeType num_samples = 256;
  const Vocoder::SizeType sample_rate = 16000;
  Vocoder vocoder(num_channels, num_samples, sample_rate);
}

TEST(VocoderTest, Process) {
  const Vocoder::SizeType num_channels = 2;
  const Vocoder::SizeType num_samples = 256;
  const Vocoder::SizeType sample_rate = 16000;
  const float scale_factor = 1.0f;

  Vocoder vocoder(num_channels, num_samples, sample_rate);
  std::vector<std::vector<float>> src(num_channels,
                                      std::vector<float>(num_samples, 0.0f));
  std::vector<std::vector<float>> out = vocoder.Process(src, scale_factor);
}

TEST(VocoderTest, RepeatProcess) {
  const Vocoder::SizeType num_channels = 2;
  const Vocoder::SizeType num_samples = 256;
  const Vocoder::SizeType sample_rate = 16000;
  const float scale_factor = 1.0f;

  Vocoder vocoder(num_channels, num_samples, sample_rate);
  std::vector<std::vector<float>> src(num_channels,
                                      std::vector<float>(num_samples, 0.0f));
  std::vector<std::vector<float>> out;
  for (int i = 0; i < 10; ++i) {
    out = vocoder.Process(src, scale_factor);
  }
}