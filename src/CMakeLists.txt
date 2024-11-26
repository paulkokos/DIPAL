cmake_minimum_required(VERSION 3.28)
project(DIPAL)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(Image)

# Add warning and error flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
target_compile_options(
    -Wall       #Enable all warnings
    -Wextra     #Enable extra warnings
    -Wpedantic  #Enable strict ISO C++ Compliance
    -Werror     # treat warnings as errors
    -g          #generate debug informations
  )

endif()
