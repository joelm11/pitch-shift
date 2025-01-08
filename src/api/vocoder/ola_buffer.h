#ifndef API_VOCODER_OLA_BUFFER_H_
#define API_VOCODER_OLA_BUFFER_H_

#include <algorithm>
#include <vector>

class OLABuffer {
 public:
  OLABuffer(size_t window_size, size_t hop_size)
      : kWindowSize_(window_size),
        kHopSize_(hop_size),
        buffer_(3 * window_size, 0.0f),
        index_(0) {}

  std::vector<float> OverlapAdd(const std::vector<float>& samples) {
    if (samples.size() != kWindowSize_) {
      throw std::invalid_argument("Samples size must be equal to window size");
    }

    // Overlap-add samples.
    for (int i = 0; i < kWindowSize_; ++i) {
      buffer_[index_ + i] += samples[i];
    }

    // Collect the output frame.
    std::vector<float> output(buffer_.begin(), buffer_.begin() + kWindowSize_);

    // Shift the buffer when we've overlapped past the frame size.
    index_ += kHopSize_;
    if (index_ >= kWindowSize_) {
      index_ -= kWindowSize_;
    }

    return output;
  }

 private:
  const size_t kWindowSize_, kHopSize_;
  std::vector<float> buffer_;
  size_t index_;
};

#endif  // API_VOCODER_OLA_BUFFER_H_