cmake_minimum_required(VERSION 3.16.3)

project(DIPAL
        VERSION 0.1.0
        LANGUAGES CXX
        DESCRIPTION "Digital Image Processing and Analysis Library"
)

# C++23 standard requirement
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build configuration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(ENABLE_SANITIZERS "Enable sanitizers in Debug" OFF)

# Global compiler warnings
if(MSVC)
  add_compile_options(/W4 /WX /permissive- /std:c++20 /Zc:__cplusplus /EHsc)
  add_compile_definitions(_SILENCE_ALL_CXX20_DEPRECATION_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)
  # Explicitly enable exceptions for MSVC
  add_compile_options(/EHsc)  # Enable C++ exceptions
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++23)
  # Explicitly enable exceptions for GCC/Clang (this is default, but let's be explicit)
  add_compile_options(-fexceptions)
endif()

# FetchContent for dependencies
include(FetchContent)

# Google Test
if(BUILD_TESTS)
  FetchContent_Declare(
          googletest
          GIT_REPOSITORY https://github.com/google/googletest.git
          GIT_TAG v1.14.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

# Expected library for C++23 (until full compiler support)
set(EXPECTED_BUILD_TESTS OFF CACHE BOOL "" FORCE)
 FetchContent_Declare(
        expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG v1.1.0
 )
 FetchContent_MakeAvailable(expected)

# Source files
file(GLOB_RECURSE DIPAL_SOURCES
        "src/*.cpp"
        "src/**/*.cpp"
)

file(GLOB_RECURSE DIPAL_HEADERS
        "include/*.hpp"
        "include/**/*.hpp"
)

# Library target
add_library(dipal ${DIPAL_SOURCES} ${DIPAL_HEADERS})

# Include directories
target_include_directories(dipal
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


# Compiler features and additional target-specific options
target_compile_features(dipal PUBLIC cxx_std_23)

# Additional MSVC-specific options for the target
if(MSVC)
  target_compile_options(dipal PRIVATE /wd4996)  # Disable specific deprecated warnings
endif()

# Sanitizers
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES Debug)
  if(NOT MSVC)
    target_compile_options(dipal PRIVATE -fsanitize=address,undefined)
    target_link_options(dipal PRIVATE -fsanitize=address,undefined)
  endif()
endif()

# Subdirectories
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Install configuration
install(TARGETS dipal
        EXPORT dipal-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(DIRECTORY include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
)










