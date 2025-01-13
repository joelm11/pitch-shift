#pragma once

#include <cstddef>
#include <vector>

template <typename T>
class OLABuffer {
 public:
  OLABuffer(const std::size_t frame_size, const std::size_t hop_size)
      : kFrameSize_(frame_size),
        kHopSize_(hop_size),
        kBufferSize_(kFrameSize_ + kHopSize_),
        buffer_(kBufferSize_, 0),
        l(0) {}

  std::vector<T> OverlapAdd(const std::vector<T>& samples) {
    if (samples.size() != kFrameSize_) {
      //   throw std::runtime_error("Samples size must be equal to window
      //   size");
    }

    // Overlap add the samples to the ring buffer.
    for (std::size_t i = 0; i < kFrameSize_; ++i) {
      buffer_[(l + i) % kBufferSize_] += samples[i];
    }

    // Copy the output frame from the buffer.
    std::vector<T> output(kHopSize_);
    for (std::size_t i = 0; i < kHopSize_; ++i) {
      output[i] = buffer_[(l + i) % kBufferSize_];
    }

    // Zero out the portion of the buffer collected for output and update the
    // write pointer.
    for (std::size_t i = 0; i < kHopSize_; ++i) {
      buffer_[(l + i) % kBufferSize_] = 0;
    }
    l = (l + kHopSize_) % kBufferSize_;

    return output;
  }

 private:
  const std::size_t kFrameSize_, kHopSize_, kBufferSize_;
  std::vector<T> buffer_;
  std::size_t l;
};