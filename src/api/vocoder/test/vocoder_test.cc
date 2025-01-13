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

  ASSERT_TRUE(output_file.save("letrollface.wav"));
}

TEST(VocoderTest2, ProcessSineWave) {
  const int kSampleRate = 48000;
  const int kNumChannels = 2;
  const int kTotalSamples = 96000;
  const int kFrameSize = 1024;
  const float kFrequency = 440.0f;  // A4 note
  const float kAmplitude = 0.5f;

  // Generate sine wave samples.
  std::vector<std::vector<float>> sine_wave(kNumChannels,
                                            std::vector<float>(kTotalSamples));
  for (int i = 0; i < kTotalSamples; ++i) {
    float sample_value =
        kAmplitude * sin(2.0f * M_PI * kFrequency * i / kSampleRate);
    for (int channel = 0; channel < kNumChannels; ++channel) {
      sine_wave[channel][i] = sample_value;
    }
  }

  // Configure vocoder.
  Vocoder vocoder(kNumChannels, kFrameSize, kSampleRate);

  // Output buffer.
  std::vector<std::vector<float>> output(kNumChannels,
                                         std::vector<float>(kTotalSamples));

  // Process sine wave in blocks of kFrameSize.
  for (int sampsProcd = 0; sampsProcd < kTotalSamples;
       sampsProcd += kFrameSize) {
    std::vector<std::vector<float>> curr_frame(kNumChannels,
                                               std::vector<float>(kFrameSize));
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < kFrameSize; ++sample) {
        curr_frame[channel][sample] = sine_wave[channel][sampsProcd + sample];
      }
    }

    auto processedChunk = vocoder.Process(curr_frame, 1.0f);

    // Write processed samples to the output buffer.
    for (int channel = 0; channel < kNumChannels; ++channel) {
      for (int sample = 0; sample < kFrameSize; ++sample) {
        output[channel][sampsProcd + sample] = processedChunk[channel][sample];
      }
    }
  }

  AudioFile<float> output_file;
  output_file.setAudioBuffer(output);
  ASSERT_TRUE(output_file.save("sine_wave_vocoded.wav"));
}