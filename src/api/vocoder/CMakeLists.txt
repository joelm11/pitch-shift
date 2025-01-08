add_library(vocoder STATIC vocoder.h vocoder.cc window.h)

add_subdirectory(fft)
target_link_libraries(vocoder PUBLIC fft_impl)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(test)
endif()