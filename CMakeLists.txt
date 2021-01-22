cmake_minimum_required(VERSION 3.16)
project(DIPAL)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(libs)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

add_executable(DIPAL main.cpp include/Image.hpp include/Image.cpp include/Segmentation.cpp include/Segmentation.hpp)