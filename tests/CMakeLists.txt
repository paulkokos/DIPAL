cmake_minimum_required(VERSION 3.20)

# Find or use Google Test
if(TARGET gtest)
    message(STATUS "Using Google Test from submodule")
else()
    # Fallback to find_package if not included as submodule
    find_package(GTest REQUIRED)
endif()

include(GoogleTest)

# Unit tests
add_executable(image_tests unit/image_tests.cpp)
target_link_libraries(image_tests PRIVATE 
    dipal
    gtest
    gtest_main
)
gtest_discover_tests(image_tests)

# Add more test executables as needed
add_executable(filter_tests unit/filter_tests.cpp)
target_link_libraries(filter_tests PRIVATE
    dipal
    gtest
    gtest_main
)
gtest_discover_tests(filter_tests)

# Integration tests
add_executable(io_tests integration/io_tests.cpp)
target_link_libraries(io_tests PRIVATE
    dipal
    gtest
    gtest_main
)
gtest_discover_tests(io_tests)
