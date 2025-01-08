#include <gtest/gtest.h>

#include <cmath>
#include <complex>
#include <vector>

#include "../fft_impl.h"

const float PI = 3.141592653589793238460;

// Naive FFT function
void NaiveFFT(std::vector<std::complex<float>>& a) {
  const size_t N = a.size();
  if (N <= 1) return;

  // Divide
  std::vector<std::complex<float>> even(N / 2);
  std::vector<std::complex<float>> odd(N / 2);
  for (size_t i = 0; i < N / 2; ++i) {
    even[i] = a[i * 2];
    odd[i] = a[i * 2 + 1];
  }

  // Conquer
  NaiveFFT(even);
  NaiveFFT(odd);

  // Combine
  for (size_t k = 0; k < N / 2; ++k) {
    std::complex<float> t = std::polar(1.f, -2.f * PI * k / N) * odd[k];
    a[k] = even[k] + t;
    a[k + N / 2] = even[k] - t;
  }
}

class FFTTest : public ::testing::Test {
 protected:
  FFTTest() {
    // Initialization code here
  }

  ~FFTTest() override {
    // Cleanup code here
  }

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test).
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor).
  }

  // Helper function to compare two complex vectors
  bool CompareComplexVectors(const std::vector<std::complex<float>>& a,
                             const std::vector<std::complex<float>>& b,
                             float tol = 1e-3) {
    if (a.size() != b.size()) {
      ADD_FAILURE() << "Vectors have different sizes: " << a.size() << " vs "
                    << b.size();
      return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
      if (std::abs(a[i] - b[i]) > tol) {
        ADD_FAILURE() << "Vectors differ at index " << i << ": " << a[i]
                      << " vs " << b[i];
        return false;
      }
    }
    return true;
  }

  const size_t kFFTLen_ = 1024;
};

// Test the forward FFT implementation against expected transform values
TEST_F(FFTTest, ForwardTransform) {
  FFTImpl fft(kFFTLen_);
  std::vector<float> input(kFFTLen_);
  float sample_rate = 1000.0;  // Sample rate in Hz
  float frequency = 100.0;     // Frequency of the sine wave in Hz

  // Populate the input vector with a 100Hz sine wave
  for (size_t i = 0; i < kFFTLen_; ++i) {
    input[i] = std::sin(2 * PI * frequency * i / sample_rate);
  }

  std::vector<std::complex<float>> output(kFFTLen_);
  fft.Forward(input, output);

  // Compute the expected output using the naive FFT function
  std::vector<std::complex<float>> expected_output(input.begin(), input.end());
  NaiveFFT(expected_output);

  // Convert expected_output to std::complex<float>
  std::vector<std::complex<float>> expected_output_float(
      expected_output.begin(), expected_output.end());

  EXPECT_TRUE(CompareComplexVectors(output, expected_output_float));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}