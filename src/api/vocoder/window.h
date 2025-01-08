#ifndef VOCODER_WINDOW_H
#define VOCODER_WINDOW_H

#include <array>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <vector>

template <typename T>
std::vector<T> ComputeHannWin(const std::size_t win_size,
                              const std::size_t an_hop_size) {
  const T kA = 0.54, kB = -0.46;

  // This is a neat little trick to normalize the window function for different
  // analysis hop sizes.
  const T kCNorm = std::sqrt((double)an_hop_size / win_size) /
                   std::sqrt(4 * kA * kA + 2 * kB * kB);

  const T kPiWin = M_PI / win_size;

  std::vector<T> window_buffer(win_size);
  for (int i = 0; i < win_size; ++i) {
    window_buffer[i] =
        kA + kB * std::cos(2 * std::numbers::pi * i / win_size) + kPiWin;
    window_buffer[i] *= kCNorm;
  }
  return window_buffer;
}
#endif