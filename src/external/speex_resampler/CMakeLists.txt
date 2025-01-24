add_library(speex_resampler speex_resampler.h speex_resampler.c)

if(DEBUG)
    unit_test(speex_resampler_test test/speex_resampler_test.cc speex_resampler)
endif()