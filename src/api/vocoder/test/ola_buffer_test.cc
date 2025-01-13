#include "../ola_buffer.h"

#include <gtest/gtest.h>

#include "AudioFile.h"

TEST(OLABufferTest, OverlapAdd0) {
  const size_t window_size = 4;
  const size_t hop_size = 0;

  // Shouldn't be able to construct an OLARBuffer with hop_size = 0.
}

TEST(OLABufferTest, OverlapAdd1) {
  const size_t window_size = 4;
  const size_t hop_size = 2;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  OLABuffer<float> buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({1.0f, 1.0f}));
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({2.0f, 2.0f}));
  }
}

TEST(OLABufferTest, OverlapAdd2) {
  const size_t window_size = 4;
  const size_t hop_size = 1;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  OLABuffer<float> buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({1.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({2.0f}));
  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({3.0f}));
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({4.0f}));
  }
}

TEST(OLABufferTest, OverlapAdd3) {
  const size_t window_size = 4;
  const size_t hop_size = 3;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  OLABuffer<float> buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples), std::vector<float>({1.0f, 1.0f, 1.0f}));
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(buffer.OverlapAdd(samples),
              std::vector<float>({2.0f, 1.0f, 1.0f}));
  }
}

TEST(OLABufferTest, OverlapAdd4) {
  const size_t window_size = 4;
  const size_t hop_size = 4;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  OLABuffer<float> buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f}));
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(buffer.OverlapAdd(samples),
              std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f}));
  }
}

TEST(OLABufferTest, OverlapAdd8) {
  const size_t window_size = 4;
  const size_t hop_size = 8;
  const std::vector<float> samples = {1.0f, 1.0f, 1.0f, 1.0f};
  OLABuffer<float> buffer(window_size, hop_size);

  EXPECT_EQ(buffer.OverlapAdd(samples),
            std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f, 0.f, 0.f, 0.f, 0.f}));
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(buffer.OverlapAdd(samples),
              std::vector<float>({1.0f, 1.0f, 1.0f, 1.0f, 0.f, 0.f, 0.f, 0.f}));
  }
}