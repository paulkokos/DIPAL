# examples/CMakeLists.txt
cmake_minimum_required(VERSION 3.18)

# Basic examples
add_executable(hello_dipal basic/hello_dipal.cpp)
target_link_libraries(hello_dipal PRIVATE dipal)

add_executable(image_io_example basic/image_io_example.cpp)
target_link_libraries(image_io_example PRIVATE dipal)

add_executable(test_filters basic/test_filters.cpp)
target_link_libraries(test_filters PRIVATE dipal)

# Advanced examples
add_executable(processing_pipeline advanced/processing_pipeline.cpp)
target_link_libraries(processing_pipeline PRIVATE dipal)
