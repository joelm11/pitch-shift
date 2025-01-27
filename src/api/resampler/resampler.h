#include <vector>

#include "resampler_base.h"
#include "speex_resampler.h"

class Resampler : public ResamplerBase {
 public:
  Resampler(const int in_rate, const int out_rate, const int num_channels,
            const int num_samples)
      : kNumChannels_(num_channels),
        kNumSamples_(num_samples),
        kNumSamplesOut_(num_samples * (float)out_rate / in_rate),
        kInRate_(in_rate),
        resampler_(
            speex_resampler_init(num_channels, in_rate, out_rate, 10, 0)),
        output_buffer_(num_channels, std::vector<float>(kNumSamplesOut_)),
        out_rate_(out_rate) {
    speex_resampler_skip_zeros(resampler_);
  }

  ~Resampler() { speex_resampler_destroy(resampler_); }

  void SetRate(const int out_rate) {
    if (out_rate_ != out_rate) {
      out_rate_ = out_rate;
      speex_resampler_set_rate(resampler_, kInRate_, out_rate);
    }
  }

  std::vector<std::vector<float>> Resample(
      const std::vector<std::vector<float>>& input) override {
    // Iterate over input channels and use resamplers to resample each channel.
    for (int i = 0; i < kNumChannels_; ++i) {
      uint32_t in_len = kNumSamples_;
      uint32_t out_len = kNumSamplesOut_;
      speex_resampler_process_float(resampler_, i, input[i].data(), &in_len,
                                    output_buffer_[i].data(), &out_len);
    }
    return output_buffer_;
  }

 private:
  const int kNumChannels_, kNumSamples_, kNumSamplesOut_, kInRate_;
  int out_rate_;
  SpeexResamplerState* resampler_;
  std::vector<std::vector<float>> output_buffer_;
};