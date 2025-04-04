cmake_minimum_required(VERSION 3.29.0)
project(pitch-shift)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DEBUG "Enable debug mode" ON)
option(RELEASE "Enable release mode" OFF)

if(DEBUG)
    message(STATUS "Debug build enabled")
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
    
    # Pull in Google Test
    message(STATUS "Unit tests enabled")
    include(FetchContent)    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY
        https://github.com/google/googletest.git
        GIT_TAG v1.15.2)
    FetchContent_MakeAvailable(GoogleTest)
    enable_testing()
    include(GoogleTest)
    include("cmake/create_ut.cmake")

    # Additional debug flags here...
else()
    set(CMAKE_BUILD_TYPE Release)   

    # Additional release flags here...
endif()

add_subdirectory(src/external)
add_subdirectory(src/api)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS pitch-shift-api)

# Conditionally build CLI.
# ...