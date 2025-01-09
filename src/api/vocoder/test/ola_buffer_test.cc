#include "../ola_buffer.h"

#include <gtest/gtest.h>

TEST(OLABufferTest, OverlapAdd) {
  const size_t window_size = 3;
  const size_t hop_size = 1;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f};
  OLABuffer buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({1.0f, 1.0f, 1.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({1.0f, 2.0f, 2.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({1.0f, 2.0f, 3.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({2.0f, 3.0f, 3.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({3.0f, 3.0f, 3.0f}));

  // for (int i = 0; i < 10; ++i) {
  //   EXPECT_EQ(buffer.OverlapAdd(samples),
  //             std::vector<float>({3.0f, 3.0f, 3.0f}));
  // }
}