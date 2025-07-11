# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.28)

include(GoogleTest)

# Test utilities (only if utils exist)
# add_library(test_utils STATIC
#     utils/TestUtils.cpp
#     utils/ImageTestUtils.cpp
# )
#
# target_link_libraries(test_utils
#     PUBLIC
#         dipal
#         GTest::gtest
# )
#
# target_include_directories(test_utils
#     PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/utils
# )

# Unit tests
function(add_dipal_test test_name)
    add_executable(${test_name} unit/${test_name}.cpp)
    target_link_libraries(${test_name}
            PRIVATE
            dipal
            GTest::gtest_main
    )
    # Use add_test instead of gtest_discover_tests for simplicity
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "unit")
endfunction()

# Define all unit tests (only add tests that exist)
# add_dipal_test(result_tests)
add_dipal_test(image_tests)
# add_dipal_test(image_io_tests)
# add_dipal_test(filter_tests)

# Integration tests (commented out until files exist)
# add_executable(integration_tests
#     integration/end_to_end_tests.cpp
#     integration/performance_tests.cpp
# )
#
# target_link_libraries(integration_tests
#     PRIVATE
#         dipal
#         test_utils
#         GTest::gtest_main
# )
#
# add_test(NAME integration_tests COMMAND integration_tests)
# set_tests_properties(integration_tests PROPERTIES LABELS "integration")

# Benchmark tests (if needed)
option(BUILD_BENCHMARKS "Build benchmark tests" OFF)
if(BUILD_BENCHMARKS)
    find_package(benchmark REQUIRED)

    add_executable(benchmarks
            benchmarks/image_processing_bench.cpp
    )

    target_link_libraries(benchmarks
            PRIVATE
            dipal
            benchmark::benchmark
    )
endif()

# Test data
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fixtures)
    file(COPY fixtures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fixtures/)
else()
    # Create empty fixtures directory
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fixtures)
endif()