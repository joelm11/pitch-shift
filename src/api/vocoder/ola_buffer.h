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
        kBufferSize_(2 * window_size),
        buffer_(kBufferSize_, 0.0f),
        start_(0),
        end_(0),
        frames_before_update_start(kWindowSize_ / kHopSize_) {}

  std::vector<float> OverlapAdd(const std::vector<float>& samples) {
    if (samples.size() != kWindowSize_) {
      throw std::runtime_error("Samples size must be equal to window size");
    }

    // Overlap-add samples.
    for (int i = 0; i < kWindowSize_; ++i) {
      buffer_[(end_ + i) % kBufferSize_] += samples[i];
    }

    // Update the start and end indices.
    end_ = (end_ + kHopSize_) % kBufferSize_;
    if (frames_before_update_start == 0) {
      start_ = (start_ + kHopSize_) % kBufferSize_;
    } else {
      --frames_before_update_start;
    }

    // Collect the output frame.
    std::vector<float> output_frame(kWindowSize_);
    for (int i = 0; i < kWindowSize_; ++i) {
      output_frame[i] = buffer_[(start_ + i) % kBufferSize_];
    }

    return output_frame;
  }

 private:
  const size_t kWindowSize_, kHopSize_, kBufferSize_;
  std::vector<float> buffer_;

  // Positions in the ring buffer. Start only begins to move after sufficient
  // frames have been added such that all frames overlapping the start frame are
  // present.
  size_t start_, end_;

  // The number of frames required before the start position can be updated is
  // given by: WindowSize / HopSize.
  // NOTE: This math is incorrect, it's most likely ceil(WindowSize / HopSize)
  // but I'll need to ensure that operation won't overwrite the head of the
  // buffer.
  size_t frames_before_update_start;
};

#endif  // API_VOCODER_OLA_BUFFER_H_