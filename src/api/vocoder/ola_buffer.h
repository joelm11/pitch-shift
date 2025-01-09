#ifndef API_VOCODER_OLA_BUFFER_H_
#define API_VOCODER_OLA_BUFFER_H_

#include <algorithm>
#include <stdexcept>
#include <vector>

class OLABuffer {
 public:
  OLABuffer(size_t window_size, size_t hop_size)
      : kWindowSize_(window_size),
        kHopSize_(hop_size),
        buffer_(3 * window_size, 0.0f),
        start_(0),
        end_(0) {}

  std::vector<float> OverlapAdd(const std::vector<float>& samples) {
    if (samples.size() != kWindowSize_) {
      throw std::runtime_error("Samples size must be equal to window size");
    }

    // Overlap-add samples.
    for (size_t i = 0; i < kWindowSize_; ++i) {
      buffer_[end_ + i] += samples[i];
    }

    // Collect the output frame.
    std::vector<float> output(buffer_.begin() + start_,
                              buffer_.begin() + start_ + kWindowSize_);

    // Update the index.
    end_ += kHopSize_;

    if (end_ >= kWindowSize_) {
      start_ = end_ - kWindowSize_;
      end_ = start_;
    }

    return output;
  }

 private:
  const size_t kWindowSize_, kHopSize_;
  std::vector<float> buffer_;
  // Start position is the index of the first valid element and is updated when
  // a valid frame is completed by successive OverlapAdd calls.
  // End position advances by HopSize and is the start position of where the
  // next frame should be added.
  size_t start_, end_;
};

#endif  // API_VOCODER_OLA_BUFFER_H_