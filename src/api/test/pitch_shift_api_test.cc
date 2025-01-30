#include "../pitch_shift_api.h"

#include <gtest/gtest.h>

#include "AudioFile.h"

// Time stretch an input .wav file.
TEST(PitchShiftApiTest, TimeStretch) {
  AudioFile<float> input_file;
  std::filesystem::path path = AUDIO_FILE_PATH;
  ASSERT_TRUE(input_file.load(path));

  // Configure the source buffer for reading from file.
  const int kNumChannels = 2;
  const int kFileTotalSamples = input_file.getNumSamplesPerChannel();
  const int kFrameSize = 256;
  const float kScaleFactor = 1.5f;
  const auto kInputSamples = input_file.samples;
  std::vector<std::vector<float>> curr_frame(kNumChannels,
                                             std::vector<float>(kFrameSize));

  // Configure pitch-shifter.
  PitchShifter shifter(kNumChannels, input_file.getSampleRate());

  // Configure output file.
  AudioFile<float> output_file;
  output_file.setNumChannels(kNumChannels);
  output_file.setNumSamplesPerChannel(kFileTotalSamples * kScaleFactor);
  output_file.setSampleRate(input_file.getSampleRate());

  // Process input file and write processed samples to output file.
  for (int i = 0, out_samps_procd = 0; i < kFileTotalSamples; i += kFrameSize) {
    // Copy samples from the input file to the current frame buffer.
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < kFrameSize; ++sample) {
        curr_frame[channel][sample] = kInputSamples[channel][i + sample];
      }
    }

    auto processedChunk = shifter.ScaleTime(curr_frame, kScaleFactor);

    // Write processed samples to the output file.
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < processedChunk.back().size(); ++sample) {
        output_file.samples[channel][out_samps_procd + sample] =
            processedChunk[channel][sample];
      }
    }
    out_samps_procd += processedChunk.back().size();
  }

  ASSERT_TRUE(output_file.save("api-stretched.wav"));
}

// Repitch an input .wav file.
TEST(PitchShiftApiTest, PitchShift) {
  AudioFile<float> input_file;
  std::filesystem::path path = AUDIO_FILE_PATH;
  ASSERT_TRUE(input_file.load(path));

  // Configure the source buffer for reading from file.
  const int kNumChannels = 2;
  const int kFileTotalSamples = input_file.getNumSamplesPerChannel();
  const int kFrameSize = 256;
  const float kScaleFactor = 1.5f;
  const auto kInputSamples = input_file.samples;
  std::vector<std::vector<float>> curr_frame(kNumChannels,
                                             std::vector<float>(kFrameSize));

  // Configure pitch-shifter.
  PitchShifter shifter(kNumChannels, input_file.getSampleRate());

  // Configure output file.
  AudioFile<float> output_file;
  output_file.setNumChannels(kNumChannels);
  output_file.setNumSamplesPerChannel(kFileTotalSamples * kScaleFactor);
  output_file.setSampleRate(input_file.getSampleRate());

  // Process input file and write processed samples to output file.
  for (int i = 0, out_samps_procd = 0; i < kFileTotalSamples; i += kFrameSize) {
    // Copy samples from the input file to the current frame buffer.
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < kFrameSize; ++sample) {
        curr_frame[channel][sample] = kInputSamples[channel][i + sample];
      }
    }

    auto processedChunk = shifter.ScalePitch(curr_frame, kScaleFactor);

    EXPECT_EQ(processedChunk[0].size(), kFrameSize);

    // Write processed samples to the output file.
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < processedChunk.back().size(); ++sample) {
        output_file.samples[channel][out_samps_procd + sample] =
            processedChunk[channel][sample];
      }
    }
    out_samps_procd += processedChunk.back().size();
  }

  ASSERT_TRUE(output_file.save("api-pitched.wav"));
}