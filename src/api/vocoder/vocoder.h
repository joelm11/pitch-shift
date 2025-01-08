#ifndef VOCODER_H_
#define VOCODER_H_

#include <vector>

#include "fft/fft_pffft.h"

class Vocoder {
 public:
  using SizeType = std::size_t;
  using FBuffer = std::vector<std::vector<float>>;
  using CFBuffer = std::vector<std::vector<std::complex<float>>>;

  Vocoder(SizeType num_channels, SizeType num_samples, SizeType sample_rate);

  std::vector<std::vector<float>> Process(
      const std::vector<std::vector<float>>& src, float scale_factor);

 private:
  void Analysis();
  void ModifyPhaseR();
  void ModifyPhaseT();
  void Synthesis();
  void ResizeBuffers(const SizeType num_channels, const SizeType num_samples);
  void InitFFT(const SizeType num_samples);

  const SizeType kNumChannels_;
  const SizeType kNumSamples_;
  const SizeType kSampleRate_;
  float scale_factor_;
  FBuffer input_buffer_, output_buffer_, window_buffer_, fft_input_buffer_,
      prev_phase_, prev_synth_phase_;
  CFBuffer fft_output_buffer_;
  std::unique_ptr<PFFFT<float>> fft_impl_;
  SizeType analysis_hop_size_;
};

#endif  // VOCODER_H_