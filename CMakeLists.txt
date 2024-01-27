cmake_minimum_required(VERSION 3.16)
project(DIPAL)
set(CMAKE_CXX_STANDARD 17)

add_library(DIPAL STATIC include/DIPAL/DIPAL.hpp src/DIPAL/DIPAL.cpp)
