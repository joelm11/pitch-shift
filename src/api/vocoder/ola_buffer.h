#ifndef API_VOCODER_OLA_BUFFER_H_
#define API_VOCODER_OLA_BUFFER_H_

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

class OLABuffer {
 public:
  OLABuffer(size_t window_size, size_t hop_size)
      : kWindowSize_(window_size),
        kHopSize_(hop_size),
        kBufferSize_(2 * window_size),
        buffer_(kBufferSize_, 0.0f),
        index_(0) {}

  std::vector<float> OverlapAdd(const std::vector<float>& samples) {
    if (samples.size() != kWindowSize_) {
      throw std::runtime_error("Samples size must be equal to window size");
    }

    // Overlap add the samples to the buffer.
    for (size_t i = 0; i < kWindowSize_; ++i) {
      buffer_[index_ + i] += samples[i];
    }
    index_ += kHopSize_;

    std::cout << "Added\n";
    for (const auto item : buffer_) {
      std::cout << item << " ";
    }
    std ::cout << " | " << index_ << " | ";
    std::cout << std::endl;

    // Copy the output frame from the buffer.
    const std::vector<float> kOutput(buffer_.begin(),
                                     buffer_.begin() + kWindowSize_);

    // If the index has moved beyond window size, we know we have a completed
    // frame and should shift the buffer, clearing the portion of the buffer
    // that does not contain a partially completed frame.
    if (index_ >= kWindowSize_) {
      // Instead of copy, use std::memmove.
      // std::copy(buffer_.begin() + kHopSize_, buffer_.end(), buffer_.begin());
      std::memmove(buffer_.data(), buffer_.data() + kHopSize_,
                   (kBufferSize_ - kHopSize_) * sizeof(float));

      std::cout << "Copied\n";
      for (const auto item : buffer_) {
        std::cout << item << " ";
      }
      std::cout << std::endl;

      std::fill(buffer_.end() - kHopSize_, buffer_.end(), 0.0f);

      index_ -= kHopSize_;

      std::cout << "Filled\n";
      for (const auto item : buffer_) {
        std::cout << item << " ";
      }
      std::cout << std::endl;
    }

    return kOutput;
  }

 private:
  const size_t kWindowSize_, kHopSize_, kBufferSize_;
  std::vector<float> buffer_;
  size_t index_;
};

#endif  // API_VOCODER_OLA_BUFFER_H_