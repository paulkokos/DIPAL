# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.16.3)

function(add_dipal_test test_name)
    add_executable(${test_name} unit/${test_name}.cpp)
    target_link_libraries(${test_name}
            PRIVATE
            dipal
            gtest_main  # Use gtest_main instead of GTest::gtest_main
    )
    # Use add_test instead of gtest_discover_tests for simplicity
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "unit")
endfunction()
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fixtures)
    file(COPY fixtures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fixtures/)
else()
    # Create empty fixtures directory
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fixtures)
endif()
# Add all unit tests
add_dipal_test(core_tests unit)
add_dipal_test(result_tests unit)
add_dipal_test(error_tests unit)
add_dipal_test(concepts_tests unit)
add_dipal_test(image_processor_tests unit)
add_dipal_test(parallel_processor_tests unit)
add_dipal_test(filter_command_tests unit)
add_dipal_test(processing_observer_tests unit)
add_dipal_test(image_io_tests unit)
add_dipal_test(bmp_io_tests unit)
add_dipal_test(ppm_io_tests unit)
add_dipal_test(filter_strategy_tests unit)
add_dipal_test(unsharp_mask_filter_tests unit)
add_dipal_test(gaussian_blur_filter_tests unit)
add_dipal_test(image_tests unit)
add_dipal_test(grayscale_image_tests unit)
add_dipal_test(color_image_tests unit)
add_dipal_test(binary_image_tests unit)
add_dipal_test(image_factory_tests unit)
add_dipal_test(image_view_tests unit)
add_dipal_test(pixel_iterator_tests unit)
add_dipal_test(memory_utils_tests unit)
add_dipal_test(concurrency_tests unit)
add_dipal_test(logger_tests unit)
add_dipal_test(interpolation_tests unit)
add_dipal_test(geometric_transform_tests unit)
add_dipal_test(resize_transform_tests unit)
add_dipal_test(median_filter_tests unit)
add_dipal_test(sobel_filter_tests unit)
add_dipal_test(filter_pipeline_tests unit)
add_dipal_test(color_conversions_tests unit)
add_dipal_test(color_space_tests unit)
add_dipal_test(profiler_tests unit)

# Add integration tests
add_dipal_test(processor_observer_integration_tests integration)
add_dipal_test(file_processing_tests integration)
add_dipal_test(image_conversion_tests integration)
add_dipal_test(filter_pipeline_integration_tests integration)
add_dipal_test(error_propagation_tests integration)
add_dipal_test(recovery_tests integration)

# Add performance tests
add_dipal_test(filter_performance_tests performance)
add_dipal_test(large_image_tests performance)
add_dipal_test(batch_processing_tests performance)
add_dipal_test(performance_regression_tests performance)

# Add stress tests
add_dipal_test(memory_stress_tests stress)
add_dipal_test(concurrent_stress_tests stress)
add_dipal_test(extreme_image_sizes_tests stress)
add_dipal_test(corrupted_data_tests stress)
add_dipal_test(boundary_condition_tests stress)

# Add compatibility tests
add_dipal_test(platform_compatibility_tests compatibility)
add_dipal_test(compiler_compatibility_tests compatibility)
add_dipal_test(cpp23_features_tests compatibility)


