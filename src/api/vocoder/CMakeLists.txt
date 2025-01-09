add_library(vocoder STATIC vocoder.h vocoder.cc window.h ola_buffer.h)

add_subdirectory(fft)
target_link_libraries(vocoder PUBLIC fft_impl)

if(DEBUG)
    add_subdirectory(test)
endif()