cmake_minimum_required(VERSION 3.16)
project(DIPAL)
set(CMAKE_CXX_STANDARD 23)
add_executable(DIPAL
        src/Image/Image.cpp
        src/Image/BinaryImage.cpp
        src/Image/GrayscaleImage.cpp
        src/Image/ColorImage.cpp

        include/Image/Image.hpp
        include/Image/BinaryImage.hpp
        include/Image/GrayscaleImage.hpp
        include/Image/ColorImage.hpp


        main.cpp
        src/Image/ImageFactory.cpp
        include/Image/ImageFactory.h
        include/Image/ImageTypes.h
)

#  You may want to customize compileer flags based on the build type. Here is how you can do it in 'CMakeLists.txt'

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
endif()

#target_compile_features(DIPAL PRIVATE cxx_std_17)
target_compile_options(DIPAL PRIVATE -Wall -Wextra)

# Enable Warnings

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(DIPAL PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
  target_compile_options(DIPAL PRIVATE /W4)  # Warning level 4 for MSVC
endif()

# Optimize for peformance
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Enable testing with CTest
enable_testing()
add_test(NAME MyTest COMMAND MyTestExecutable)




