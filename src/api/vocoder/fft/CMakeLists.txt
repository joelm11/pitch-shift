add_library(fft INTERFACE fft_base.h fft_pffft.h)

target_include_directories(fft INTERFACE ${PRETTY_FAST_FFT_SOURCE_DIR})
target_link_libraries(fft INTERFACE pffft)

if(DEBUG)
    add_subdirectory(test)
endif()