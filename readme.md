# Pitch Shift

A modern C++ library for real-time audio pitch shifting and time stretching. Designed for flexibility and high performance, Pitch Shift provides a simple API for manipulating audio buffers, with all dependencies managed automatically via CMake.

## Features

- High-quality pitch shifting and time stretching
- Simple, modern C++ API
- Multi-channel audio support
- All dependencies (including GoogleTest and AudioFile) are automatically pulled and built via CMake

## Getting Started

### Building the Project

Pitch Shift uses CMake for configuration and builds. All dependencies are automatically downloaded and built as part of the process—no manual setup required.

```sh
git clone --recurse-submodules https://github.com/joelm11/pitch-shift
cd pitch-shift
cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build
```

### Running Tests

Unit tests are built and run using GoogleTest, which is included automatically.

```sh
cd build
ctest
```

## Usage

### API Overview

Include the main header:

```cpp
#include "pitch_shift_api.h"
```

#### Pitch Shifting Example

```cpp
#include "pitch_shift_api.h"
#include "AudioFile.h"

AudioFile<float> input_file, output_file;
input_file.load("input.wav");


PitchShifter shifter(/*num_channels=*/2, input_file.getSampleRate());

float pitch_factor = 1.5f; // Raise pitch by 1.5x

for (const auto& frame : input_file.samples) {
    auto processed = shifter.ScalePitch(input_file.samples, pitch_factor);
    output_file.write(processed);
}

```

#### Building the API

This project is meant to be consumed by CMake as a subdirectory after being cloned or added as a submodule e.g./
```
add_subdirectory(<path_to_pitch-shift>)
target_link_libraries(my_app PUBLIC pitch-shift-api)
```

## Dependencies

- [AudioFile](https://github.com/adamstark/AudioFile) (for reading/writing WAV files)
- [GoogleTest](https://github.com/google/googletest) (for unit testing)
- [KissFFT](https://github.com/mborgerding/kissfft) (for FFT operations)
- [Speex Resampler](https://github.com/xiph/speexdsp) (for resampling)

All dependencies are automatically downloaded and built by CMake. No manual installation is required.
