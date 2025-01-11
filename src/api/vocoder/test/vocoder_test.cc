#include "../vocoder.h"

#include <gtest/gtest.h>

#include "AudioFile.h"

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

TEST(VocoderTest2, ProcessAudioFile) {
  AudioFile<float> input_file;
  std::filesystem::path path = AUDIO_FILE_PATH;
  ASSERT_TRUE(input_file.load(path));

  // Configure the source buffer for reading from file.
  const int kNumChannels = input_file.getNumChannels();
  const int kFileTotalSamples = input_file.getNumSamplesPerChannel();
  const int kFrameSize = 1024;
  const auto kInputSamples = input_file.samples;
  std::vector<std::vector<float>> curr_frame(kNumChannels,
                                             std::vector<float>(kFrameSize));

  // Configure vocoder.
  Vocoder vocoder(kNumChannels, kFrameSize, input_file.getSampleRate());

  // Configure output file.
  AudioFile<float> output_file;
  output_file.setNumChannels(kNumChannels);
  output_file.setNumSamplesPerChannel(kFileTotalSamples);
  output_file.setSampleRate(input_file.getSampleRate());
  // auto output_samples = output_file.samples;
  // ASSERT_TRUE(output_samples.size() == kNumChannels);
  // ASSERT_TRUE(output_samples[0].size() == kFileTotalSamples);

  // Process input file and write processed samples to output file.
  for (int sampsProcd = 0; sampsProcd < kFileTotalSamples;
       sampsProcd += kFrameSize) {
    // Copy samples from the input file to the current frame buffer.
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < kFrameSize; ++sample) {
        curr_frame[channel][sample] =
            kInputSamples[channel][sampsProcd + sample];
      }
    }

    auto processedChunk = vocoder.Process(curr_frame, 1.0f);

    // Write processed samples to the output file.
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < kFrameSize; ++sample) {
        output_file.samples[channel][sampsProcd + sample] =
            processedChunk[channel][sample];
      }
    }
  }

  // Debug: Check if save function is called and its result
  bool save_result = output_file.save("letrollface.wav");
  std::cout << "Save function called, result: " << save_result << std::endl;
  ASSERT_TRUE(save_result);
}