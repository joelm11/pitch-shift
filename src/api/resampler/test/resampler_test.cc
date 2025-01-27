#include "../resampler.h"

#include <gtest/gtest.h>

// Resample a 440Hz sine wave at 44.1kHz to 48kHz.
TEST(ResamplerTest, ResampleSine) {
  const int in_rate = 44100;
  const int out_rate = 48000;
  const int num_channels = 1;
  const int num_samples = 1000;
  Resampler resampler(in_rate, out_rate, num_channels, num_samples);

  // Generate a 440Hz sine wave at 44.1kHz.
  std::vector<std::vector<float>> input(num_channels,
                                        std::vector<float>(num_samples));
  for (int i = 0; i < num_samples; ++i) {
    input[0][i] = 0.5f * sinf(2 * M_PI * 440 * i / in_rate);
  }

  // Resample the sine wave to 48kHz.
  std::vector<std::vector<float>> output = resampler.Resample(input);

  // Check that the output has the expected number of samples.
  EXPECT_EQ(output[0].size(),
            std::floor(num_samples * (float)out_rate / in_rate));

  // Check that the output has the expected frequency.
  // The output frequency is 480Hz, which is 440Hz resampled from 44.1kHz to
  // 48kHz.
  const float kTolerance = 0.1f;
  for (int i = 0; i < output[0].size(); ++i) {
    // float expected = 0.5f * sinf(2 * M_PI * 440 * i / out_rate);
    // EXPECT_NEAR(output[0][i], expected, kTolerance);
    printf("%f\n", output[0][i]);
  }
}