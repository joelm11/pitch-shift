#include "../speex_resampler.h"

#include <gtest/gtest.h>

TEST(SpeexResamplerTest, SpeexResamplerState) {
  SpeexResamplerState* st = speex_resampler_init(1, 44100, 48000, 10, NULL);
  ASSERT_TRUE(st != NULL);
  speex_resampler_destroy(st);
}