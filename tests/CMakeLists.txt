# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.18)

# Find Google Test
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

# Add more test targets as needed

# Set tests output directory
set_target_properties(image_tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)
