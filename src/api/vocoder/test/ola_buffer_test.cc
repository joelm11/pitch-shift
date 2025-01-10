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

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(buffer.OverlapAdd(samples),
              std::vector<float>({3.0f, 3.0f, 3.0f}));
  }
}

TEST(OLABufferTest, OverlapAddDifferentWindowSize) {
  const size_t window_size = 4;
  const size_t hop_size = 2;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  OLABuffer buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 2.0f, 2.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({2.0f, 2.0f, 2.0f, 2.0f}));

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(buffer.OverlapAdd(samples),
              std::vector<float>({2.0f, 2.0f, 2.0f, 2.0f}));
  }
}

TEST(OLABufferTest, OverlapAddDifferentHopSize) {
  const size_t window_size = 4;
  const size_t hop_size = 3;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  const std::vector<std::vector<float>> results = {
      {1.f, 1.f, 1.f, 1.f},
      {1.f, 1.f, 1.f, 2.f},
      {2.f, 1.f, 1.f, 2.f},
      {2.f, 1.f, 1.f, 2.f},

  };
  OLABuffer buffer(window_size, hop_size);

  for (const auto& result : results) {
    EXPECT_EQ(buffer.OverlapAdd(samples), result);
  }
}

TEST(OLABufferTest, NoOverlapAdd) {
  const size_t window_size = 4;
  const size_t hop_size = 4;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  OLABuffer buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f}));
}