cmake_minimum_required(VERSION 3.20)

# Basic examples
add_executable(hello_dipal basic/hello_dipal.cpp)
target_link_libraries(hello_dipal PRIVATE dipal)

# TODO: Add more example executables
