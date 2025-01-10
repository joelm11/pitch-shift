#include "../vocoder.h"

#include <gtest/gtest.h>

class VocoderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    num_channels = 2;
    num_samples = 256;
    sample_rate = 16000;
    scale_factor = 1.0f;
    vocoder = std::make_unique<Vocoder>(num_channels, num_samples, sample_rate);
    src = std::vector<std::vector<float>>(
        num_channels, std::vector<float>(num_samples, 0.0f));
  }

  Vocoder::SizeType num_channels;
  Vocoder::SizeType num_samples;
  Vocoder::SizeType sample_rate;
  float scale_factor;
  std::unique_ptr<Vocoder> vocoder;
  std::vector<std::vector<float>> src;
};

TEST_F(VocoderTest, Constructor) {
  EXPECT_NO_THROW(Vocoder vocoder(num_channels, num_samples, sample_rate));
}

TEST_F(VocoderTest, Process) {
  std::vector<std::vector<float>> out = vocoder->Process(src, scale_factor);
  EXPECT_EQ(out.size(), num_channels);
  EXPECT_EQ(out[0].size(), num_samples);
}

TEST_F(VocoderTest, RepeatProcess) {
  std::vector<std::vector<float>> out;
  for (int i = 0; i < 10; ++i) {
    out = vocoder->Process(src, scale_factor);
  }
  EXPECT_EQ(out.size(), num_channels);
  EXPECT_EQ(out[0].size(), num_samples);
}

TEST_F(VocoderTest, ChangingChannelCount) {
  std::vector<std::vector<float>> new_src(
      num_channels + 1, std::vector<float>(num_samples, 0.0f));
  EXPECT_THROW(vocoder->Process(new_src, scale_factor), std::runtime_error);
}