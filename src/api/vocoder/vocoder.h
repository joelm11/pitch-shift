#ifndef VOCODER_H_
#define VOCODER_H_

#include <vector>

#include "fft/fft_impl.h"
#include "ola_buffer.h"
#include "window.h"

class Vocoder {
 public:
  using SizeType = std::size_t;
  using FBuffer = std::vector<std::vector<float>>;
  using CFBuffer = std::vector<std::vector<std::complex<float>>>;

  Vocoder(SizeType num_channels, SizeType num_samples, SizeType sample_rate);

  std::vector<std::vector<float>> Process(
      const std::vector<std::vector<float>>& src, float scale_factor);

 private:
  void ValidateAndUpdate(const std::vector<std::vector<float>>& src,
                         const float scale_factor);
  void Analysis();
  void ModifyPhaseR();
  void ModifyPhaseT();
  void Synthesis();
  void InitBuffers(const SizeType num_channels, const SizeType num_samples,
                   const SizeType synthesis_hop_size);
  void InitFFT(const SizeType num_channels, const SizeType num_samples);

  const SizeType kNumChannels_;
  const SizeType kNumSamples_;
  const SizeType kSampleRate_;
  const SizeType kAnalysisHopSize = kNumSamples_ / 4;
  const std::vector<float> kWindowBuffer_ =
      ComputeHannWin<float>(kNumSamples_, kAnalysisHopSize);

  FBuffer input_buffer_, output_buffer_, fft_input_buffer_, prev_phase_,
      prev_synth_phase_;
  CFBuffer fft_output_buffer_;
  // NOTE: Using a vector of 1D FFTs for simplicity.
  // TODO: Update to use a multidimensional FFT implementation later.
  std::vector<std::unique_ptr<FFTImpl>> ffts_;
  std::vector<OLABuffer> olabuffers_;
  SizeType synthesis_hop_size_;
  float scale_factor_;
};

#endif  // VOCODER_H_