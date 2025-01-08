#include "../fft_pffft.h"

#include <gtest/gtest.h>

#include <cmath>
#include <vector>

// Helper function to compare two vectors
bool vectorsAreEqual(const std::vector<float>& a, const std::vector<float>& b,
                     float epsilon = 1e-5) {
  if (a.size() != b.size()) return false;
  for (size_t i = 0; i < a.size(); ++i) {
    if (std::fabs(a[i] - b[i]) > epsilon) return false;
  }
  return true;
}

// Test fixture for FFT tests
class FFTTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Initialize FFT library if needed
  }

  void TearDown() override {
    // Clean up FFT library if needed
  }
};

// Test case for forward FFT
TEST_F(FFTTest, ForwardFFT) {
  std::vector<float> input = {1.0, 0.0, 0.0, 0.0};
  std::vector<float> expected_output = {1.0, 1.0, 1.0, 1.0};
  std::vector<float> output(4);

  // Perform FFT
  fft_pffft_forward(input.data(), output.data(), 4);

  // Validate output
  EXPECT_TRUE(vectorsAreEqual(output, expected_output));
}

// Test case for inverse FFT
TEST_F(FFTTest, InverseFFT) {
  std::vector<float> input = {1.0, 1.0, 1.0, 1.0};
  std::vector<float> expected_output = {1.0, 0.0, 0.0, 0.0};
  std::vector<float> output(4);

  // Perform inverse FFT
  fft_pffft_inverse(input.data(), output.data(), 4);

  // Validate output
  EXPECT_TRUE(vectorsAreEqual(output, expected_output));
}

// Test case for round-trip FFT (forward followed by inverse)
TEST_F(FFTTest, RoundTripFFT) {
  std::vector<float> input = {1.0, 2.0, 3.0, 4.0};
  std::vector<float> temp(4);
  std::vector<float> output(4);

  // Perform forward FFT
  fft_pffft_forward(input.data(), temp.data(), 4);

  // Perform inverse FFT
  fft_pffft_inverse(temp.data(), output.data(), 4);

  // Validate output
  EXPECT_TRUE(vectorsAreEqual(output, input));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
