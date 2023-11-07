cmake_minimum_required(VERSION 3.16)
project(DIPAL)
set(SOURCE_FILES main.cpp)
set(CMAKE_CXX_STANDARD 17)

#include_directories(../DIPAL/include/DIPAL)
#
#link_directories(DIPAL)
#
#add_subdirectory(include)
#
#target_link_libraries(${PROJECT_NAME} ${SOURCE_FILES})

#add_library(DIPAL_LIBRARY src/main.cpp)
#add_library()
add_executable(MAIN main.cpp)