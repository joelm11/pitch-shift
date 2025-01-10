#include "vocoder.h"

Vocoder::Vocoder(const SizeType num_channels, const SizeType num_samples,
                 const SizeType sample_rate)
    : kNumChannels_(num_channels),
      kNumSamples_(num_samples),
      kSampleRate_(sample_rate),
      synthesis_hop_size_(kAnalysisHopSize) {
  InitBuffers(num_channels, num_samples, synthesis_hop_size_);
  InitFFT(num_channels, num_samples);
}

std::vector<std::vector<float>> Vocoder::Process(
    const std::vector<std::vector<float>>& src, const float scale_factor) {
  Validate(src, scale_factor);
  Analysis();
  ModifyPhaseR();
  // ModifyPhaseT
  Synthesis();

  return output_buffer_;
}

void Vocoder::Validate(const std::vector<std::vector<float>>& src,
                       const float scale_factor) {
  // If the channel count changes, throw an error.
  if (src.size() != kNumChannels_) {
    throw std::runtime_error("Vocoder: Channel count mismatch.");
  }
  // If the scale factor changes, update synthesis hop size.
  if (scale_factor != scale_factor_) {
    scale_factor_ = scale_factor;
    synthesis_hop_size_ = kAnalysisHopSize * scale_factor_;
    // TODO: Make this a method that resets the buffer for the new hop-size.
    olabuffers_ = std::vector<OLABuffer>(
        kNumChannels_, OLABuffer(kNumSamples_, synthesis_hop_size_));
  }
  // If the sample count changed, pad with zeros.
  if (src[0].size() < kNumSamples_) {
    for (int i = 0; i < kNumChannels_; ++i) {
      input_buffer_[i] = std::move(src[i]);
      input_buffer_[i].resize(kNumSamples_, 0.0f);
    }
  } else {
    input_buffer_ = std::move(src);
  }
}

void Vocoder::Analysis() {
  // Apply window function.
  for (int i = 0; i < kNumChannels_; ++i) {
    for (int j = 0; j < kNumSamples_; ++j) {
      input_buffer_[i][j] *= kWindowBuffer_[j];
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

void Vocoder::Synthesis() {
  // Inverse FFT.
  for (int i = 0; i < kNumChannels_; ++i) {
    ffts_[i]->Inverse(fft_output_buffer_[i], output_buffer_[i]);
  }
  // Apply window function.
  for (int i = 0; i < kNumChannels_; ++i) {
    for (int j = 0; j < kNumSamples_; ++j) {
      output_buffer_[i][j] *= kWindowBuffer_[j];
    }
  }
  // Overlap-add.
  for (int i = 0; i < kNumChannels_; ++i) {
    output_buffer_[i] = olabuffers_[i].OverlapAdd(output_buffer_[i]);
  }
}

void Vocoder::InitBuffers(const SizeType num_channels,
                          const SizeType num_samples,
                          const SizeType synthesis_hop_size) {
  // Initialize internal buffers.
  input_buffer_ = FBuffer(num_channels, std::vector<float>(num_samples));
  output_buffer_ = FBuffer(num_channels, std::vector<float>(num_samples));
  prev_phase_ = FBuffer(num_channels, std::vector<float>(num_samples));
  prev_synth_phase_ = FBuffer(num_channels, std::vector<float>(num_samples));
  fft_input_buffer_ = FBuffer(num_channels, std::vector<float>(num_samples));
  fft_output_buffer_ =
      CFBuffer(num_channels, std::vector<std::complex<float>>(num_samples));
  olabuffers_ = std::vector<OLABuffer>(
      num_channels, OLABuffer(num_samples, synthesis_hop_size));
};

void Vocoder::InitFFT(const SizeType num_channels, const SizeType num_samples) {
  ffts_.clear();
  for (int i = 0; i < num_channels; ++i) {
    ffts_.push_back(std::make_unique<FFTImpl>(num_samples));
  }
}