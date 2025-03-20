cmake_minimum_required(VERSION 3.18)

# Find GTest
find_package(GTest REQUIRED)
include(GoogleTest)

# Unit tests
add_executable(image_tests unit/image_tests.cpp)
target_link_libraries(image_tests PRIVATE dipal GTest::GTest GTest::Main)
gtest_discover_tests(image_tests)

# TODO: Add more test executables
