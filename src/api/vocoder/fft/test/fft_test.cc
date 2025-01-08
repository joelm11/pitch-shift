#include <gtest/gtest.h>

#include <complex>
#include <vector>

#include "../fft_impl.h"  // Assuming fft_impl.h contains the FFT implementation

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
  bool CompareComplexVectors(const std::vector<std::complex<double>>& a,
                             const std::vector<std::complex<double>>& b,
                             double tol = 1e-9) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
      if (std::abs(a[i] - b[i]) > tol) return false;
    }
    return true;
  }
};

// Test the construction and destruction of the FFT implementation
TEST_F(FFTTest, ConstructionDestruction) {
  FFTImpl fft;
  // No assertions needed, just ensuring no exceptions are thrown
}

// Test the forward FFT implementation against expected transform values
TEST_F(FFTTest, ForwardTransform) {
  FFTImpl fft;
  std::vector<std::complex<double>> input = {1.0, 1.0, 1.0, 1.0};
  std::vector<std::complex<double>> expected_output = {4.0, 0.0, 0.0, 0.0};
  std::vector<std::complex<double>> output = fft.forward(input);
  EXPECT_TRUE(CompareComplexVectors(output, expected_output));
}

// Test the inverse FFT implementation and ensure it's the same as the initial
// input
TEST_F(FFTTest, InverseTransform) {
  FFTImpl fft;
  std::vector<std::complex<double>> input = {4.0, 0.0, 0.0, 0.0};
  std::vector<std::complex<double>> expected_output = {1.0, 1.0, 1.0, 1.0};
  std::vector<std::complex<double>> output = fft.inverse(input);
  EXPECT_TRUE(CompareComplexVectors(output, expected_output));
}

// Test the transformation in both directions
TEST_F(FFTTest, ForwardAndInverseTransform) {
  FFTImpl fft;
  std::vector<std::complex<double>> input = {1.0, 2.0, 3.0, 4.0};
  std::vector<std::complex<double>> forward_output = fft.forward(input);
  std::vector<std::complex<double>> inverse_output =
      fft.inverse(forward_output);
  EXPECT_TRUE(CompareComplexVectors(input, inverse_output));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}