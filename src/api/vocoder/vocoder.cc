#include "vocoder.h"

#include "window.h"

Vocoder::Vocoder(const SizeType num_channels, const SizeType num_samples,
                 const SizeType sample_rate)
    : kNumChannels_(num_channels),
      kNumSamples_(num_samples),
      kSampleRate_(sample_rate) {
  ResizeBuffers(num_channels, num_samples);
  InitFFT(num_channels, num_samples);
  // Calculate constants based on scale factor.
  // Initialize window function.
  // -- Actually, let's calculate this every run and allow it to be dynamic.
}

std::vector<std::vector<float>> Vocoder::Process(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  // Catch case where buffer changes.
  // Catch case where scale factor changes.
  // Catch case where zero-padding is required.

  Analysis();

  // ModifyPhaseR

  // ModifyPhaseT
  // Synthesis
  return std::vector<std::vector<float>>();
}

void Vocoder::Analysis() {
  // Apply window function.
  for (int i = 0; i < kNumChannels_; ++i) {
    for (int j = 0; j < kNumSamples_; ++j) {
      input_buffer_[i][j] *= window_buffer_[0][j];
    }
  }
  // FFT
  for (int i = 0; i < kNumChannels_; ++i) {
    ffts_[i]->Forward(input_buffer_[i], fft_output_buffer_[i]);
  }
  // Convert FFT result to magnitude and phase format.
  for (int i = 0; i < kNumChannels_; ++i) {
    for (int j = 0; j < kNumSamples_; ++j) {
      fft_output_buffer_[i][j] = std::polar(std::abs(fft_output_buffer_[i][j]),
                                            std::arg(fft_output_buffer_[i][j]));
    }
  }
}

void Vocoder::ModifyPhaseR() {
  for (int i = 0; i < kNumChannels_; ++i) {
    for (int j = 0; j < fft_output_buffer_.size(); ++j) {
      fft_output_buffer_[i][j].imag(0);
    }
  }
}

void Vocoder::ResizeBuffers(const SizeType num_channels,
                            const SizeType num_samples) {
  // Initialize internal buffers.
  input_buffer_ = FBuffer(num_channels, std::vector<float>(num_samples));
  output_buffer_ = FBuffer(num_channels, std::vector<float>(num_samples));
  window_buffer_ = FBuffer(1, std::vector<float>(num_samples));
  prev_phase_ = FBuffer(num_channels, std::vector<float>(num_samples));
  prev_synth_phase_ = FBuffer(num_channels, std::vector<float>(num_samples));
  fft_input_buffer_ = FBuffer(num_channels, std::vector<float>(num_samples));
  fft_output_buffer_ =
      CFBuffer(num_channels, std::vector<std::complex<float>>(num_samples));
};

void Vocoder::InitFFT(const SizeType num_channels, const SizeType num_samples) {
  // Initialize FFTs.
  for (int i = 0; i < num_channels; ++i) {
    ffts_[i] = std::make_unique<FFTImpl>(num_samples);
  }
}