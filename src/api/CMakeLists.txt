add_library(pitch-shift-api pitch_shift_api.h pitch_shift_api.cc)

add_subdirectory(vocoder)
add_subdirectory(resampler)

target_link_libraries(pitch-shift-api PUBLIC vocoder resampler)

# Build unit tests if in debug mode.
if(DEBUG)
    add_subdirectory(test)
endif()