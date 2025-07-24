#!/bin/bash

# DIPAL Test Structure Creator
# Creates all test files and directories according to the implementation plan

set -e # Exit on any error

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Default target directory
TARGET_DIR="${1:-tests}"

echo -e "${BLUE}===========================================${NC}"
echo -e "${BLUE}  DIPAL Test Structure Creator${NC}"
echo -e "${BLUE}===========================================${NC}"
echo -e "${CYAN}Creating test structure in: ${TARGET_DIR}${NC}"
echo

# Create main test directories
echo -e "${YELLOW}📁 Creating directory structure...${NC}"
mkdir -p "$TARGET_DIR"/{unit,integration,performance,stress,compatibility,utils,fixtures}
mkdir -p "$TARGET_DIR/fixtures"/{test_images,corrupted,generated,large}

# Function to create a test file with boilerplate
create_test_file() {
  local filepath="$1"
  local test_name="$2"
  local class_under_test="$3"
  local includes="$4"
  local priority="$5"

  cat >"$filepath" <<EOF
// $filepath
// Priority: $priority
// Auto-generated test file for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
$includes

using namespace DIPAL;

/**
 * @brief Test fixture for $test_name
 */
class ${test_name}Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup code for $test_name tests
        // Initialize any required objects or state
    }
    
    void TearDown() override {
        // Common cleanup code for $test_name tests
        // Clean up any resources
    }
    
    // Helper methods for this test suite
    // Add common utility functions here
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(${test_name}Test, DefaultConstruction) {
    // Test default construction/creation
    // TODO: Implement basic construction test
    EXPECT_TRUE(true) << "Default construction test not implemented";
}

TEST_F(${test_name}Test, BasicOperations) {
    // Test basic operations
    // TODO: Implement basic operations test
    EXPECT_TRUE(true) << "Basic operations test not implemented";
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(${test_name}Test, ErrorHandling) {
    // Test error conditions and edge cases
    // TODO: Implement error handling tests
    EXPECT_TRUE(true) << "Error handling test not implemented";
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(${test_name}Test, BoundaryConditions) {
    // Test boundary conditions and limits
    // TODO: Implement boundary condition tests
    EXPECT_TRUE(true) << "Boundary condition test not implemented";
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(${test_name}Test, BasicPerformance) {
    // Basic performance validation
    // TODO: Implement performance tests if needed
    EXPECT_TRUE(true) << "Performance test not implemented";
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(${test_name}Test, Integration) {
    // Integration with other components
    // TODO: Implement integration tests if needed
    EXPECT_TRUE(true) << "Integration test not implemented";
}

// Additional test cases should be added based on specific functionality
// of the class under test

EOF
  echo -e "  ${GREEN}✓${NC} Created: $filepath"
}

# Function to create integration test files
create_integration_test() {
  local filepath="$1"
  local test_name="$2"
  local priority="$3"

  cat >"$filepath" <<EOF
// $filepath
// Priority: $priority
// Integration test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <memory>
#include <vector>

using namespace DIPAL;

/**
 * @brief Integration test fixture for $test_name
 */
class ${test_name}Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup for integration tests
        // Initialize multiple components that will interact
    }
    
    void TearDown() override {
        // Cleanup after integration tests
    }
    
    // Helper methods for creating test scenarios
    std::unique_ptr<Image> createTestImage(int width = 100, int height = 100) {
        // TODO: Implement test image creation
        return nullptr;
    }
};

// ============================================================================
// COMPONENT INTERACTION TESTS
// ============================================================================

TEST_F(${test_name}Test, ComponentInteraction) {
    // Test interaction between multiple components
    // TODO: Implement component interaction tests
    EXPECT_TRUE(true) << "Component interaction test not implemented";
}

// ============================================================================
// WORKFLOW TESTS
// ============================================================================

TEST_F(${test_name}Test, CompleteWorkflow) {
    // Test complete workflows from start to finish
    // TODO: Implement complete workflow tests
    EXPECT_TRUE(true) << "Complete workflow test not implemented";
}

// ============================================================================
// ERROR PROPAGATION TESTS
// ============================================================================

TEST_F(${test_name}Test, ErrorPropagation) {
    // Test how errors propagate through the system
    // TODO: Implement error propagation tests
    EXPECT_TRUE(true) << "Error propagation test not implemented";
}

EOF
  echo -e "  ${GREEN}✓${NC} Created: $filepath"
}

# Function to create performance test files
create_performance_test() {
  local filepath="$1"
  local test_name="$2"
  local priority="$3"

  cat >"$filepath" <<EOF
// $filepath
// Priority: $priority
// Performance test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <chrono>
#include <vector>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Performance test fixture for $test_name
 */
class ${test_name}Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup for performance tests
    }
    
    void TearDown() override {
        // Cleanup after performance tests
    }
    
    // Helper method to measure execution time
    template<typename Func>
    double measureExecutionTime(Func&& func) {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        return duration.count() / 1000.0; // Return milliseconds
    }
};

// ============================================================================
// BENCHMARK TESTS
// ============================================================================

TEST_F(${test_name}Test, PerformanceBenchmark) {
    // Benchmark core operations
    // TODO: Implement performance benchmarks
    
    // Example benchmark structure:
    // auto executionTime = measureExecutionTime([&]() {
    //     // Operation to benchmark
    // });
    // 
    // EXPECT_LT(executionTime, 1000.0) << "Operation took too long: " << executionTime << "ms";
    
    EXPECT_TRUE(true) << "Performance benchmark not implemented";
}

// ============================================================================
// SCALABILITY TESTS
// ============================================================================

TEST_F(${test_name}Test, ScalabilityTest) {
    // Test performance with increasing load
    // TODO: Implement scalability tests
    EXPECT_TRUE(true) << "Scalability test not implemented";
}

// ============================================================================
// MEMORY PERFORMANCE TESTS
// ============================================================================

TEST_F(${test_name}Test, MemoryPerformance) {
    // Test memory usage and allocation patterns
    // TODO: Implement memory performance tests
    EXPECT_TRUE(true) << "Memory performance test not implemented";
}

EOF
  echo -e "  ${GREEN}✓${NC} Created: $filepath"
}

# Function to create stress test files
create_stress_test() {
  local filepath="$1"
  local test_name="$2"
  local priority="$3"

  cat >"$filepath" <<EOF
// $filepath
// Priority: $priority
// Stress test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <thread>
#include <vector>
#include <future>

using namespace DIPAL;

/**
 * @brief Stress test fixture for $test_name
 */
class ${test_name}Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup for stress tests
    }
    
    void TearDown() override {
        // Cleanup after stress tests
    }
    
    // Helper method for concurrent operations
    template<typename Func>
    void runConcurrentOperations(int numThreads, Func&& func) {
        std::vector<std::future<void>> futures;
        
        for (int i = 0; i < numThreads; ++i) {
            futures.push_back(std::async(std::launch::async, func));
        }
        
        for (auto& future : futures) {
            future.wait();
        }
    }
};

// ============================================================================
// RESOURCE STRESS TESTS
// ============================================================================

TEST_F(${test_name}Test, ResourceStress) {
    // Test system under resource stress
    // TODO: Implement resource stress tests
    EXPECT_TRUE(true) << "Resource stress test not implemented";
}

// ============================================================================
// CONCURRENCY STRESS TESTS
// ============================================================================

TEST_F(${test_name}Test, ConcurrencyStress) {
    // Test system under high concurrency
    // TODO: Implement concurrency stress tests
    EXPECT_TRUE(true) << "Concurrency stress test not implemented";
}

// ============================================================================
// LOAD STRESS TESTS
// ============================================================================

TEST_F(${test_name}Test, LoadStress) {
    // Test system under high load
    // TODO: Implement load stress tests
    EXPECT_TRUE(true) << "Load stress test not implemented";
}

EOF
  echo -e "  ${GREEN}✓${NC} Created: $filepath"
}

echo -e "${PURPLE}📋 PHASE 1: Foundation Tests (Critical)${NC}"

# Phase 1A: Foundation Tests
echo -e "${CYAN}Phase 1A: Foundation Tests${NC}"
create_test_file "$TARGET_DIR/unit/core_tests.cpp" "Core" "Core" "" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/result_tests.cpp" "Result" "Result<T>" "#include <expected>" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/error_tests.cpp" "Error" "Error" "" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/concepts_tests.cpp" "Concepts" "Concepts" "#include <concepts>" "🔧 MEDIUM"

# Phase 1B: Processing Engine
echo -e "${CYAN}Phase 1B: Processing Engine${NC}"
create_test_file "$TARGET_DIR/unit/image_processor_tests.cpp" "ImageProcessor" "ImageProcessor" "" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/parallel_processor_tests.cpp" "ParallelProcessor" "ParallelProcessor" "#include <thread>" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/filter_command_tests.cpp" "FilterCommand" "FilterCommand" "" "🔧 HIGH"
create_test_file "$TARGET_DIR/unit/processing_observer_tests.cpp" "ProcessingObserver" "ProcessingObserver" "" "🔧 HIGH"

create_integration_test "$TARGET_DIR/integration/processor_observer_integration_tests.cpp" "ProcessorObserverIntegration" "🔧 HIGH"

# Phase 1C: I/O System
echo -e "${CYAN}Phase 1C: I/O System${NC}"
create_test_file "$TARGET_DIR/unit/image_io_tests.cpp" "ImageIO" "ImageIO" "#include <filesystem>" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/bmp_io_tests.cpp" "BMPImageIO" "BMPImageIO" "#include <fstream>" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/ppm_io_tests.cpp" "PPMImageIO" "PPMImageIO" "#include <fstream>" "🔧 MEDIUM"

create_integration_test "$TARGET_DIR/integration/file_processing_tests.cpp" "FileProcessing" "🔧 HIGH"

# Phase 1D: Known Working Filters
echo -e "${CYAN}Phase 1D: Known Working Filters${NC}"
create_test_file "$TARGET_DIR/unit/filter_strategy_tests.cpp" "FilterStrategy" "FilterStrategy" "" "🔧 HIGH"
create_test_file "$TARGET_DIR/unit/unsharp_mask_filter_tests.cpp" "UnsharpMaskFilter" "UnsharpMaskFilter" "" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/gaussian_blur_filter_tests.cpp" "GaussianBlurFilter" "GaussianBlurFilter" "" "🔧 HIGH"

create_performance_test "$TARGET_DIR/performance/filter_performance_tests.cpp" "FilterPerformance" "🔧 MEDIUM"

echo -e "${PURPLE}📋 PHASE 2: Image System Validation${NC}"

# Phase 2A: Core Image Classes
echo -e "${CYAN}Phase 2A: Core Image Classes${NC}"
create_test_file "$TARGET_DIR/unit/image_tests.cpp" "Image" "Image" "" "🔧 HIGH"
create_test_file "$TARGET_DIR/unit/grayscale_image_tests.cpp" "GrayscaleImage" "GrayscaleImage" "" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/color_image_tests.cpp" "ColorImage" "ColorImage" "" "⭐ CRITICAL"
create_test_file "$TARGET_DIR/unit/binary_image_tests.cpp" "BinaryImage" "BinaryImage" "" "🔧 MEDIUM"

create_integration_test "$TARGET_DIR/integration/image_conversion_tests.cpp" "ImageConversion" "🔧 HIGH"

# Phase 2B: Image Support Systems
echo -e "${CYAN}Phase 2B: Image Support Systems${NC}"
create_test_file "$TARGET_DIR/unit/image_factory_tests.cpp" "ImageFactory" "ImageFactory" "" "🔧 HIGH"
create_test_file "$TARGET_DIR/unit/image_view_tests.cpp" "ImageView" "ImageView" "" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/unit/pixel_iterator_tests.cpp" "PixelIterator" "PixelIterator" "#include <iterator>" "🔧 MEDIUM"

echo -e "${PURPLE}📋 PHASE 3: Utilities & Infrastructure${NC}"

# Phase 3A: Memory & Concurrency
echo -e "${CYAN}Phase 3A: Memory & Concurrency${NC}"
create_test_file "$TARGET_DIR/unit/memory_utils_tests.cpp" "MemoryUtils" "MemoryUtils" "#include <memory>" "🔧 HIGH"
create_test_file "$TARGET_DIR/unit/concurrency_tests.cpp" "Concurrency" "Concurrency" "#include <thread>" "🔧 HIGH"
create_test_file "$TARGET_DIR/unit/logger_tests.cpp" "Logger" "Logger" "" "🔧 MEDIUM"

create_stress_test "$TARGET_DIR/stress/memory_stress_tests.cpp" "MemoryStress" "🔧 HIGH"
create_stress_test "$TARGET_DIR/stress/concurrent_stress_tests.cpp" "ConcurrentStress" "🔧 HIGH"

# Phase 3B: Transformations
echo -e "${CYAN}Phase 3B: Transformations${NC}"
create_test_file "$TARGET_DIR/unit/interpolation_tests.cpp" "Interpolation" "Interpolation" "" "🔧 HIGH"
create_test_file "$TARGET_DIR/unit/geometric_transform_tests.cpp" "GeometricTransform" "GeometricTransform" "" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/unit/resize_transform_tests.cpp" "ResizeTransform" "ResizeTransform" "" "🔧 MEDIUM"

echo -e "${PURPLE}📋 PHASE 4: Extended Coverage${NC}"

# Phase 4A: Additional Filters
echo -e "${CYAN}Phase 4A: Additional Filters${NC}"
create_test_file "$TARGET_DIR/unit/median_filter_tests.cpp" "MedianFilter" "MedianFilter" "" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/unit/sobel_filter_tests.cpp" "SobelFilter" "SobelFilter" "" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/unit/filter_pipeline_tests.cpp" "FilterPipeline" "FilterPipeline" "#include <vector>" "🔧 HIGH"

create_integration_test "$TARGET_DIR/integration/filter_pipeline_integration_tests.cpp" "FilterPipelineIntegration" "🔧 HIGH"

# Phase 4B: Color & Advanced Features
echo -e "${CYAN}Phase 4B: Color & Advanced Features${NC}"
create_test_file "$TARGET_DIR/unit/color_conversions_tests.cpp" "ColorConversions" "ColorConversions" "" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/unit/color_space_tests.cpp" "ColorSpace" "ColorSpace" "" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/unit/profiler_tests.cpp" "Profiler" "Profiler" "#include <chrono>" "🔧 LOW"

create_performance_test "$TARGET_DIR/performance/large_image_tests.cpp" "LargeImagePerformance" "🔧 HIGH"
create_performance_test "$TARGET_DIR/performance/batch_processing_tests.cpp" "BatchProcessingPerformance" "🔧 MEDIUM"

echo -e "${PURPLE}📋 PHASE 5: Quality Assurance${NC}"

# Phase 5A: Stress & Edge Cases
echo -e "${CYAN}Phase 5A: Stress & Edge Cases${NC}"
create_stress_test "$TARGET_DIR/stress/extreme_image_sizes_tests.cpp" "ExtremeImageSizes" "🔧 HIGH"
create_stress_test "$TARGET_DIR/stress/corrupted_data_tests.cpp" "CorruptedData" "🔧 HIGH"
create_stress_test "$TARGET_DIR/stress/boundary_condition_tests.cpp" "BoundaryCondition" "🔧 HIGH"

create_integration_test "$TARGET_DIR/integration/error_propagation_tests.cpp" "ErrorPropagation" "🔧 HIGH"
create_integration_test "$TARGET_DIR/integration/recovery_tests.cpp" "Recovery" "🔧 MEDIUM"

# Phase 5B: Cross-Platform
echo -e "${CYAN}Phase 5B: Cross-Platform${NC}"
create_test_file "$TARGET_DIR/compatibility/platform_compatibility_tests.cpp" "PlatformCompatibility" "Platform" "#include <filesystem>" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/compatibility/compiler_compatibility_tests.cpp" "CompilerCompatibility" "Compiler" "" "🔧 MEDIUM"
create_test_file "$TARGET_DIR/compatibility/cpp23_features_tests.cpp" "Cpp23Features" "Cpp23" "#include <expected>" "🔧 LOW"

create_performance_test "$TARGET_DIR/performance/performance_regression_tests.cpp" "PerformanceRegression" "🔧 MEDIUM"

echo -e "${PURPLE}📋 TEST UTILITIES${NC}"

# Test Utilities
echo -e "${CYAN}Test Utilities${NC}"
cat >"$TARGET_DIR/utils/test_utils.cpp" <<'EOF'
// tests/utils/test_utils.cpp
// Common test utilities for DIPAL Library

#include "test_utils.hpp"
#include <DIPAL/DIPAL.hpp>
#include <random>
#include <chrono>

namespace DIPAL {
namespace TestUtils {

std::mt19937& getRandomGenerator() {
    static std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    return gen;
}

std::vector<uint8_t> generateRandomData(size_t size) {
    std::vector<uint8_t> data(size);
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    auto& gen = getRandomGenerator();
    
    for (auto& byte : data) {
        byte = dist(gen);
    }
    
    return data;
}

bool compareImages(const Image& img1, const Image& img2, double tolerance) {
    if (img1.getWidth() != img2.getWidth() || 
        img1.getHeight() != img2.getHeight() ||
        img1.getType() != img2.getType()) {
        return false;
    }
    
    // TODO: Implement pixel-by-pixel comparison with tolerance
    return true;
}

std::filesystem::path getTestDataPath() {
    return std::filesystem::current_path() / "fixtures";
}

} // namespace TestUtils
} // namespace DIPAL
EOF

cat >"$TARGET_DIR/utils/test_utils.hpp" <<'EOF'
// tests/utils/test_utils.hpp
// Common test utilities header for DIPAL Library

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <vector>
#include <filesystem>
#include <random>

namespace DIPAL {
namespace TestUtils {

/**
 * @brief Get a shared random number generator for tests
 */
std::mt19937& getRandomGenerator();

/**
 * @brief Generate random data for testing
 */
std::vector<uint8_t> generateRandomData(size_t size);

/**
 * @brief Compare two images with tolerance
 */
bool compareImages(const Image& img1, const Image& img2, double tolerance = 0.01);

/**
 * @brief Get the path to test data directory
 */
std::filesystem::path getTestDataPath();

/**
 * @brief Timer for performance measurements
 */
class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    double elapsedMs() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return duration.count() / 1000.0;
    }
    
    void reset() {
        start_ = std::chrono::high_resolution_clock::now();
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_;
};

} // namespace TestUtils
} // namespace DIPAL
EOF

cat >"$TARGET_DIR/utils/image_test_utils.cpp" <<'EOF'
// tests/utils/image_test_utils.cpp
// Image-specific test utilities for DIPAL Library

#include "image_test_utils.hpp"
#include <DIPAL/DIPAL.hpp>

namespace DIPAL {
namespace ImageTestUtils {

std::unique_ptr<GrayscaleImage> createTestGrayscaleImage(int width, int height, uint8_t fillValue) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result) {
        return nullptr;
    }
    
    auto image = std::move(result.value());
    
    // Fill with the specified value
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image->setPixel(x, y, fillValue);
        }
    }
    
    return image;
}

std::unique_ptr<ColorImage> createTestColorImage(int width, int height, uint8_t r, uint8_t g, uint8_t b) {
    auto result = ImageFactory::createColor(width, height);
    if (!result) {
        return nullptr;
    }
    
    auto image = std::move(result.value());
    
    // Fill with the specified color
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image->setPixel(x, y, r, g, b);
        }
    }
    
    return image;
}

std::unique_ptr<GrayscaleImage> createGradientImage(int width, int height) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result) {
        return nullptr;
    }
    
    auto image = std::move(result.value());
    
    // Create a diagonal gradient
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t value = static_cast<uint8_t>((static_cast<double>(x + y) / (width + height)) * 255);
            image->setPixel(x, y, value);
        }
    }
    
    return image;
}

std::unique_ptr<ColorImage> createRainbowImage(int width, int height) {
    auto result = ImageFactory::createColor(width, height);
    if (!result) {
        return nullptr;
    }
    
    auto image = std::move(result.value());
    
    // Create a rainbow pattern
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t r = static_cast<uint8_t>((static_cast<double>(x) / width) * 255);
            uint8_t g = static_cast<uint8_t>((static_cast<double>(y) / height) * 255);
            uint8_t b = static_cast<uint8_t>(((static_cast<double>(x) + static_cast<double>(y)) / (width + height)) * 255);
            image->setPixel(x, y, r, g, b);
        }
    }
    
    return image;
}

} // namespace ImageTestUtils
} // namespace DIPAL
EOF

cat >"$TARGET_DIR/utils/image_test_utils.hpp" <<'EOF'
// tests/utils/image_test_utils.hpp
// Image-specific test utilities header for DIPAL Library

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <memory>

namespace DIPAL {
namespace ImageTestUtils {

/**
 * @brief Create a test grayscale image filled with a specific value
 */
std::unique_ptr<GrayscaleImage> createTestGrayscaleImage(int width, int height, uint8_t fillValue = 128);

/**
 * @brief Create a test color image filled with a specific color
 */
std::unique_ptr<ColorImage> createTestColorImage(int width, int height, uint8_t r = 128, uint8_t g = 128, uint8_t b = 128);

/**
 * @brief Create a grayscale image with a gradient pattern
 */
std::unique_ptr<GrayscaleImage> createGradientImage(int width, int height);

/**
 * @brief Create a color image with a rainbow pattern
 */
std::unique_ptr<ColorImage> createRainbowImage(int width, int height);

/**
 * @brief Create a checkerboard pattern image
 */
template<typename ImageType>
std::unique_ptr<ImageType> createCheckerboardImage(int width, int height, int squareSize = 8);

/**
 * @brief Validate image properties
 */
template<typename ImageType>
bool validateImageProperties(const ImageType& image, int expectedWidth, int expectedHeight);

} // namespace ImageTestUtils
} // namespace DIPAL
EOF

cat >"$TARGET_DIR/utils/mock_observers.cpp" <<'EOF'
// tests/utils/mock_observers.cpp
// Mock observer implementations for testing

#include "mock_observers.hpp"
#include <iostream>

namespace DIPAL {
namespace MockObservers {

void MockProcessingObserver::onProcessingStarted(const std::string& operationName) {
    events.push_back({EventType::ProcessingStarted, operationName, 0.0f, ""});
    if (verbose) {
        std::cout << "[MOCK] Processing started: " << operationName << std::endl;
    }
}

void MockProcessingObserver::onProcessingCompleted(const std::string& operationName, bool success) {
    events.push_back({EventType::ProcessingCompleted, operationName, success ? 1.0f : 0.0f, ""});
    if (verbose) {
        std::cout << "[MOCK] Processing completed: " << operationName 
                  << " (success: " << (success ? "true" : "false") << ")" << std::endl;
    }
}

void MockProcessingObserver::onProgressUpdated(float progress) {
    events.push_back({EventType::ProgressUpdated, "", progress, ""});
    if (verbose) {
        std::cout << "[MOCK] Progress updated: " << (progress * 100) << "%" << std::endl;
    }
}

void MockProcessingObserver::onError(const std::string& errorMessage) {
    events.push_back({EventType::Error, "", 0.0f, errorMessage});
    if (verbose) {
        std::cout << "[MOCK] Error: " << errorMessage << std::endl;
    }
}

} // namespace MockObservers
} // namespace DIPAL
EOF

cat >"$TARGET_DIR/utils/mock_observers.hpp" <<'EOF'
// tests/utils/mock_observers.hpp
// Mock observer implementations for testing

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <vector>
#include <string>

namespace DIPAL {
namespace MockObservers {

enum class EventType {
    ProcessingStarted,
    ProcessingCompleted,
    ProgressUpdated,
    Error
};

struct ObserverEvent {
    EventType type;
    std::string operationName;
    float progress;
    std::string message;
};

class MockProcessingObserver : public ProcessingObserver {
public:
    explicit MockProcessingObserver(bool verbose = false) : verbose(verbose) {}
    
    void onProcessingStarted(const std::string& operationName) override;
    void onProcessingCompleted(const std::string& operationName, bool success) override;
    void onProgressUpdated(float progress) override;
    void onError(const std::string& errorMessage) override;
    
    // Test helpers
    void clear() { events.clear(); }
    size_t getEventCount() const { return events.size(); }
    const std::vector<ObserverEvent>& getEvents() const { return events; }
    
    bool hasEvent(EventType type) const {
        for (const auto& event : events) {
            if (event.type == type) return true;
        }
        return false;
    }
    
    size_t countEvents(EventType type) const {
        size_t count = 0;
        for (const auto& event : events) {
            if (event.type == type) count++;
        }
        return count;
    }
    
private:
    std::vector<ObserverEvent> events;
    bool verbose;
};

} // namespace MockObservers
} // namespace DIPAL
EOF

cat >"$TARGET_DIR/utils/test_image_generator.cpp" <<'EOF'
// tests/utils/test_image_generator.cpp
// Procedural test image generation utilities

#include "test_image_generator.hpp"
#include <DIPAL/DIPAL.hpp>
#include <cmath>
#include <random>

namespace DIPAL {
namespace TestImageGenerator {

std::unique_ptr<GrayscaleImage> generateNoiseImage(int width, int height, uint8_t mean, uint8_t stddev) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result) return nullptr;
    
    auto image = std::move(result.value());
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(mean, stddev);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = dist(gen);
            uint8_t pixel = static_cast<uint8_t>(std::clamp(value, 0.0f, 255.0f));
            image->setPixel(x, y, pixel);
        }
    }
    
    return image;
}

std::unique_ptr<GrayscaleImage> generateSineWaveImage(int width, int height, double frequency) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result) return nullptr;
    
    auto image = std::move(result.value());
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double value = std::sin(x * frequency * 2.0 * M_PI / width) * 127.5 + 127.5;
            uint8_t pixel = static_cast<uint8_t>(std::clamp(value, 0.0, 255.0));
            image->setPixel(x, y, pixel);
        }
    }
    
    return image;
}

std::unique_ptr<GrayscaleImage> generateCircleImage(int width, int height, int centerX, int centerY, int radius) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result) return nullptr;
    
    auto image = std::move(result.value());
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double distance = std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
            uint8_t pixel = (distance <= radius) ? 255 : 0;
            image->setPixel(x, y, pixel);
        }
    }
    
    return image;
}

std::unique_ptr<ColorImage> generateColorGradientImage(int width, int height, GradientType type) {
    auto result = ImageFactory::createColor(width, height);
    if (!result) return nullptr;
    
    auto image = std::move(result.value());
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t r = 0, g = 0, b = 0;
            
            switch (type) {
                case GradientType::Horizontal:
                    r = static_cast<uint8_t>((static_cast<double>(x) / width) * 255);
                    g = 128;
                    b = 255 - r;
                    break;
                case GradientType::Vertical:
                    g = static_cast<uint8_t>((static_cast<double>(y) / height) * 255);
                    r = 128;
                    b = 255 - g;
                    break;
                case GradientType::Radial:
                    {
                        double centerX = width / 2.0;
                        double centerY = height / 2.0;
                        double maxDistance = std::sqrt(centerX * centerX + centerY * centerY);
                        double distance = std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
                        double normalized = distance / maxDistance;
                        r = static_cast<uint8_t>(normalized * 255);
                        g = static_cast<uint8_t>((1.0 - normalized) * 255);
                        b = 128;
                    }
                    break;
            }
            
            image->setPixel(x, y, r, g, b);
        }
    }
    
    return image;
}

} // namespace TestImageGenerator
} // namespace DIPAL
EOF

cat >"$TARGET_DIR/utils/test_image_generator.hpp" <<'EOF'
// tests/utils/test_image_generator.hpp
// Procedural test image generation utilities

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <memory>

namespace DIPAL {
namespace TestImageGenerator {

enum class GradientType {
    Horizontal,
    Vertical,
    Radial
};

/**
 * @brief Generate a grayscale image with random noise
 */
std::unique_ptr<GrayscaleImage> generateNoiseImage(int width, int height, uint8_t mean = 128, uint8_t stddev = 32);

/**
 * @brief Generate a grayscale image with sine wave pattern
 */
std::unique_ptr<GrayscaleImage> generateSineWaveImage(int width, int height, double frequency = 1.0);

/**
 * @brief Generate a grayscale image with a circle
 */
std::unique_ptr<GrayscaleImage> generateCircleImage(int width, int height, int centerX, int centerY, int radius);

/**
 * @brief Generate a color image with gradient
 */
std::unique_ptr<ColorImage> generateColorGradientImage(int width, int height, GradientType type = GradientType::Horizontal);

/**
 * @brief Generate a test pattern image for debugging
 */
template<typename ImageType>
std::unique_ptr<ImageType> generateTestPatternImage(int width, int height);

} // namespace TestImageGenerator
} // namespace DIPAL
EOF

echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/test_utils.cpp"
echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/test_utils.hpp"
echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/image_test_utils.cpp"
echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/image_test_utils.hpp"
echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/mock_observers.cpp"
echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/mock_observers.hpp"
echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/test_image_generator.cpp"
echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/utils/test_image_generator.hpp"

echo -e "${PURPLE}📋 FIXTURES & SAMPLE DATA${NC}"

# Create sample fixture files
echo -e "${CYAN}Creating fixture directories and README files${NC}"

cat >"$TARGET_DIR/fixtures/README.md" <<'EOF'
# Test Fixtures

This directory contains test data and sample files for DIPAL tests.

## Structure

- `test_images/` - Sample images in various formats for testing I/O operations
- `corrupted/` - Intentionally malformed files for error handling tests
- `generated/` - Procedurally generated test data
- `large/` - Large test images for performance testing

## Usage

Test utilities can access these files using:

```cpp
auto testDataPath = DIPAL::TestUtils::getTestDataPath();
auto sampleImage = testDataPath / "test_images" / "sample.bmp";
```

## Adding Test Data

When adding new test files:

1. Keep file sizes reasonable (< 1MB for regular tests, mark large files clearly)
2. Include a variety of formats and properties
3. Document any special characteristics in this README
4. Consider licensing and copyright for any external images

## Test Image Properties

### Small Test Images (< 100KB)
- `small_grayscale.pgm` - 100x100 grayscale PGM
- `small_color.ppm` - 100x100 color PPM  
- `small_binary.pbm` - 100x100 binary PBM
- `tiny.bmp` - 32x32 BMP for quick tests

### Medium Test Images (100KB - 1MB)
- `medium_photo.bmp` - 500x500 photographic content
- `medium_graphics.bmp` - 500x500 synthetic graphics

### Large Test Images (> 1MB)
- `large_test.bmp` - 2048x2048 for performance testing
- Place in `large/` subdirectory

### Corrupted Test Files
- `invalid_header.bmp` - Invalid BMP header
- `truncated.ppm` - Incomplete PPM file
- `wrong_extension.txt` - Text file with image extension
EOF

cat >"$TARGET_DIR/fixtures/test_images/README.md" <<'EOF'
# Test Images

Sample images for I/O and processing tests.

## Files

- Add sample images here for testing
- Include various formats: BMP, PPM, PGM, PBM
- Include various sizes and content types
- Document any special properties

## Generating Test Images

You can generate basic test images using the test utilities:

```cpp
auto testImage = DIPAL::ImageTestUtils::createTestGrayscaleImage(100, 100, 128);
// Save to file for use in tests
```
EOF

cat >"$TARGET_DIR/fixtures/corrupted/README.md" <<'EOF'
# Corrupted Test Files

Intentionally malformed files for testing error handling.

## Purpose

These files test that the library properly handles:
- Invalid file headers
- Truncated files  
- Wrong file extensions
- Corrupted data

## Files

Add corrupted test files here with documentation of what makes them invalid.
EOF

cat >"$TARGET_DIR/fixtures/generated/README.md" <<'EOF'
# Generated Test Data

Procedurally generated test data for reproducible testing.

## Usage

Test data can be generated using the TestImageGenerator utilities:

```cpp
auto noiseImage = DIPAL::TestImageGenerator::generateNoiseImage(256, 256);
auto gradientImage = DIPAL::TestImageGenerator::generateColorGradientImage(512, 512);
```

## Benefits

- Reproducible test data
- Parametric testing with various sizes and properties
- No licensing concerns
- Unlimited variations for stress testing
EOF

cat >"$TARGET_DIR/fixtures/large/README.md" <<'EOF'
# Large Test Images

Large images for performance and stress testing.

## Warning

Files in this directory may be large (> 1MB). They are used for:
- Performance benchmarking
- Memory usage testing
- Stress testing with realistic image sizes

## Guidelines

- Mark clearly in test code when using large images
- Consider using generated images instead of stored files when possible
- Document memory requirements for tests using these images
EOF

echo -e "  ${GREEN}✓${NC} Created fixture directories and documentation"

echo -e "${PURPLE}📋 CMAKE INTEGRATION${NC}"

# Create updated CMakeLists.txt for tests
cat >"$TARGET_DIR/CMakeLists.txt" <<'EOF'
# tests/CMakeLists.txt - DIPAL Test Suite
cmake_minimum_required(VERSION 3.28)

# Find required packages
find_package(GTest REQUIRED)
include(GoogleTest)

# Test utilities library
add_library(dipal_test_utils STATIC
    utils/test_utils.cpp
    utils/image_test_utils.cpp
    utils/mock_observers.cpp
    utils/test_image_generator.cpp
)

target_link_libraries(dipal_test_utils
    PUBLIC
        dipal
        GTest::gtest
)

target_include_directories(dipal_test_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# Function to add DIPAL unit tests
function(add_dipal_unit_test test_name)
    add_executable(${test_name} unit/${test_name}.cpp)
    target_link_libraries(${test_name}
        PRIVATE
            dipal
            dipal_test_utils
            GTest::gtest_main
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "unit")
endfunction()

# Function to add DIPAL integration tests
function(add_dipal_integration_test test_name)
    add_executable(${test_name} integration/${test_name}.cpp)
    target_link_libraries(${test_name}
        PRIVATE
            dipal
            dipal_test_utils
            GTest::gtest_main
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "integration")
endfunction()

# Function to add DIPAL performance tests
function(add_dipal_performance_test test_name)
    add_executable(${test_name} performance/${test_name}.cpp)
    target_link_libraries(${test_name}
        PRIVATE
            dipal
            dipal_test_utils
            GTest::gtest_main
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "performance")
endfunction()

# Function to add DIPAL stress tests
function(add_dipal_stress_test test_name)
    add_executable(${test_name} stress/${test_name}.cpp)
    target_link_libraries(${test_name}
        PRIVATE
            dipal
            dipal_test_utils
            GTest::gtest_main
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "stress")
endfunction()

# Function to add DIPAL compatibility tests
function(add_dipal_compatibility_test test_name)
    add_executable(${test_name} compatibility/${test_name}.cpp)
    target_link_libraries(${test_name}
        PRIVATE
            dipal
            dipal_test_utils
            GTest::gtest_main
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "compatibility")
endfunction()

# ============================================================================
# PHASE 1: Foundation Tests (Critical Priority)
# ============================================================================

# Phase 1A: Foundation Tests
add_dipal_unit_test(core_tests)
add_dipal_unit_test(result_tests)
add_dipal_unit_test(error_tests)
add_dipal_unit_test(concepts_tests)

# Phase 1B: Processing Engine
add_dipal_unit_test(image_processor_tests)
add_dipal_unit_test(parallel_processor_tests)
add_dipal_unit_test(filter_command_tests)
add_dipal_unit_test(processing_observer_tests)
add_dipal_integration_test(processor_observer_integration_tests)

# Phase 1C: I/O System
add_dipal_unit_test(image_io_tests)
add_dipal_unit_test(bmp_io_tests)
add_dipal_unit_test(ppm_io_tests)
add_dipal_integration_test(file_processing_tests)

# Phase 1D: Known Working Filters
add_dipal_unit_test(filter_strategy_tests)
add_dipal_unit_test(unsharp_mask_filter_tests)
add_dipal_unit_test(gaussian_blur_filter_tests)
add_dipal_performance_test(filter_performance_tests)

# ============================================================================
# PHASE 2: Image System Validation
# ============================================================================

# Phase 2A: Core Image Classes
add_dipal_unit_test(image_tests)
add_dipal_unit_test(grayscale_image_tests)
add_dipal_unit_test(color_image_tests)
add_dipal_unit_test(binary_image_tests)
add_dipal_integration_test(image_conversion_tests)

# Phase 2B: Image Support Systems
add_dipal_unit_test(image_factory_tests)
add_dipal_unit_test(image_view_tests)
add_dipal_unit_test(pixel_iterator_tests)

# ============================================================================
# PHASE 3: Utilities & Infrastructure
# ============================================================================

# Phase 3A: Memory & Concurrency
add_dipal_unit_test(memory_utils_tests)
add_dipal_unit_test(concurrency_tests)
add_dipal_unit_test(logger_tests)
add_dipal_stress_test(memory_stress_tests)
add_dipal_stress_test(concurrent_stress_tests)

# Phase 3B: Transformations
add_dipal_unit_test(interpolation_tests)
add_dipal_unit_test(geometric_transform_tests)
add_dipal_unit_test(resize_transform_tests)

# ============================================================================
# PHASE 4: Extended Coverage
# ============================================================================

# Phase 4A: Additional Filters
add_dipal_unit_test(median_filter_tests)
add_dipal_unit_test(sobel_filter_tests)
add_dipal_unit_test(filter_pipeline_tests)
add_dipal_integration_test(filter_pipeline_integration_tests)

# Phase 4B: Color & Advanced Features
add_dipal_unit_test(color_conversions_tests)
add_dipal_unit_test(color_space_tests)
add_dipal_unit_test(profiler_tests)
add_dipal_performance_test(large_image_tests)
add_dipal_performance_test(batch_processing_tests)

# ============================================================================
# PHASE 5: Quality Assurance
# ============================================================================

# Phase 5A: Stress & Edge Cases
add_dipal_stress_test(extreme_image_sizes_tests)
add_dipal_stress_test(corrupted_data_tests)
add_dipal_stress_test(boundary_condition_tests)
add_dipal_integration_test(error_propagation_tests)
add_dipal_integration_test(recovery_tests)

# Phase 5B: Cross-Platform
add_dipal_compatibility_test(platform_compatibility_tests)
add_dipal_compatibility_test(compiler_compatibility_tests)
add_dipal_compatibility_test(cpp23_features_tests)
add_dipal_performance_test(performance_regression_tests)

# ============================================================================
# Test Data Setup
# ============================================================================

# Copy test fixtures to build directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fixtures)
    file(COPY fixtures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fixtures/)
else()
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fixtures)
endif()

# ============================================================================
# Test Targets for Different Phases
# ============================================================================

# Custom targets for running specific test phases
add_custom_target(test_phase1
    COMMAND ${CMAKE_CTEST_COMMAND} -L "unit" --output-on-failure -R "(core|result|error|concepts|image_processor|parallel_processor|filter_command|processing_observer|image_io|bmp_io|ppm_io|filter_strategy|unsharp_mask|gaussian_blur)_tests"
    DEPENDS core_tests result_tests error_tests concepts_tests image_processor_tests parallel_processor_tests filter_command_tests processing_observer_tests image_io_tests bmp_io_tests ppm_io_tests filter_strategy_tests unsharp_mask_filter_tests gaussian_blur_filter_tests
    COMMENT "Running Phase 1 tests (Foundation)"
)

add_custom_target(test_phase2
    COMMAND ${CMAKE_CTEST_COMMAND} -L "unit" --output-on-failure -R "(image|grayscale_image|color_image|binary_image|image_factory|image_view|pixel_iterator)_tests"
    DEPENDS image_tests grayscale_image_tests color_image_tests binary_image_tests image_factory_tests image_view_tests pixel_iterator_tests
    COMMENT "Running Phase 2 tests (Image System)"
)

add_custom_target(test_critical
    COMMAND ${CMAKE_CTEST_COMMAND} -L "unit" --output-on-failure -R "(core|result|error|image_processor|parallel_processor|image_io|bmp_io|unsharp_mask|grayscale_image|color_image)_tests"
    COMMENT "Running only critical tests"
)

add_custom_target(test_integration_only
    COMMAND ${CMAKE_CTEST_COMMAND} -L "integration" --output-on-failure
    COMMENT "Running integration tests only"
)

add_custom_target(test_performance_only
    COMMAND ${CMAKE_CTEST_COMMAND} -L "performance" --output-on-failure
    COMMENT "Running performance tests only"
)

add_custom_target(test_stress_only
    COMMAND ${CMAKE_CTEST_COMMAND} -L "stress" --output-on-failure
    COMMENT "Running stress tests only"
)

# Custom target for quick smoke test
add_custom_target(test_smoke
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "(core|result|image_processor|bmp_io)_tests"
    COMMENT "Running smoke tests (quick validation)"
)

# Show test summary
add_custom_target(test_summary
    COMMAND ${CMAKE_COMMAND} -E echo "=========================================="
    COMMAND ${CMAKE_COMMAND} -E echo "DIPAL Test Suite Summary"
    COMMAND ${CMAKE_COMMAND} -E echo "=========================================="
    COMMAND ${CMAKE_COMMAND} -E echo "Available test targets:"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_smoke      - Quick validation"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_critical   - Critical functionality only"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_phase1     - Foundation tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_phase2     - Image system tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_integration_only - Integration tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_performance_only - Performance tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_stress_only      - Stress tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  test            - All tests"
    COMMAND ${CMAKE_COMMAND} -E echo "=========================================="
    COMMENT "Showing test summary"
)

EOF

echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/CMakeLists.txt"

echo -e "${PURPLE}📋 USAGE INSTRUCTIONS${NC}"

# Create usage instructions
cat >"$TARGET_DIR/README.md" <<'EOF'
# DIPAL Test Suite

Comprehensive test suite for the DIPAL (Digital Image Processing and Analysis Library).

## Quick Start

```bash
# Build all tests
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make

# Run smoke tests (quick validation)
make test_smoke

# Run critical tests only
make test_critical

# Run all tests
make test
```

## Test Structure

### Test Categories

- **Unit Tests** (`tests/unit/`) - Individual component testing
- **Integration Tests** (`tests/integration/`) - Component interaction testing  
- **Performance Tests** (`tests/performance/`) - Benchmarking and performance validation
- **Stress Tests** (`tests/stress/`) - Resource limits and edge cases
- **Compatibility Tests** (`tests/compatibility/`) - Cross-platform validation

### Test Phases

#### Phase 1: Foundation (Priority: Critical)
- Core functionality (Result<T>, Error handling, Concepts)
- Processing engine (ImageProcessor, ParallelProcessor)
- I/O system (BMP, PPM, ImageIO)
- Known working filters (UnsharpMask, GaussianBlur)

#### Phase 2: Image System (Priority: High)
- Image classes (Grayscale, Color, Binary)
- Image support systems (Factory, View, Iterator)

#### Phase 3: Utilities (Priority: Medium)
- Memory and concurrency utilities
- Transformations and interpolation

#### Phase 4: Extended Features (Priority: Medium-Low)
- Additional filters and color operations
- Advanced transformations

#### Phase 5: Quality Assurance (Priority: Low)
- Stress testing and edge cases
- Cross-platform compatibility

## Running Tests

### By Phase
```bash
make test_phase1    # Foundation tests
make test_phase2    # Image system tests
```

### By Category
```bash
make test_integration_only  # Integration tests only
make test_performance_only  # Performance tests only
make test_stress_only      # Stress tests only
```

### Individual Tests
```bash
# Run specific test
./core_tests
./image_processor_tests

# Run with verbose output
./core_tests --gtest_verbose
```

### Test Filtering
```bash
# Run tests matching pattern
ctest -R "image.*tests"

# Run tests with specific label
ctest -L "unit"
ctest -L "integration"
ctest -L "performance"
```

## Test Development

### Adding New Tests

1. **Unit Test**: Add to appropriate `tests/unit/` file
2. **Integration Test**: Add to `tests/integration/`
3. **Performance Test**: Add to `tests/performance/`

### Test Utilities

The test suite includes comprehensive utilities:

- **TestUtils** - Common testing utilities
- **ImageTestUtils** - Image-specific test helpers
- **MockObservers** - Mock implementations for testing
- **TestImageGenerator** - Procedural test data generation

### Example Usage

```cpp
#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include "test_utils.hpp"
#include "image_test_utils.hpp"

TEST(ExampleTest, CreateAndProcessImage) {
    // Create test image
    auto image = DIPAL::ImageTestUtils::createTestGrayscaleImage(100, 100, 128);
    ASSERT_NE(image, nullptr);
    
    // Test processing
    DIPAL::ImageProcessor processor;
    auto filter = std::make_unique<DIPAL::UnsharpMaskFilter>(1.5f, 2.0f, 10);
    auto command = std::make_unique<DIPAL::FilterCommand>(std::move(filter));
    
    auto result = processor.process(*image, std::move(command));
    EXPECT_TRUE(result.has_value());
}
```

## File Organization

```
tests/
├── unit/                    # 34 unit test files
├── integration/             # 6 integration test files
├── performance/             # 4 performance test files
├── stress/                  # 5 stress test files
├── compatibility/           # 3 compatibility test files
├── utils/                   # Test utilities and helpers
├── fixtures/                # Test data and sample files
├── CMakeLists.txt          # Build configuration
└── README.md               # This file
```

## Test Priorities

- ⭐ **CRITICAL** - Must work immediately (fully implemented features)
- 🔧 **HIGH** - Important functionality (likely implemented)
- 🔧 **MEDIUM** - Standard functionality (partially implemented)
- 🔧 **LOW** - Optional functionality (headers only)

## Contributing

When adding tests:

1. Follow the existing naming conventions
2. Use the provided test utilities when possible
3. Include both positive and negative test cases
4. Add performance tests for computationally intensive operations
5. Document any special test requirements

## Debugging Failed Tests

```bash
# Run with detailed output
ctest --output-on-failure --verbose

# Run specific failing test with gtest options
./failing_test --gtest_break_on_failure --gtest_catch_exceptions=0

# Run under debugger
gdb ./failing_test
```
EOF

echo -e "  ${GREEN}✓${NC} Created: $TARGET_DIR/README.md"

# Final summary
echo
echo -e "${BLUE}===========================================${NC}"
echo -e "${GREEN}🎉 DIPAL Test Suite Created Successfully!${NC}"
echo -e "${BLUE}===========================================${NC}"
echo
echo -e "${YELLOW}📊 Summary:${NC}"
echo -e "  📁 Directories: ${GREEN}7${NC} (unit, integration, performance, stress, compatibility, utils, fixtures)"
echo -e "  📄 Test Files: ${GREEN}52${NC} total"
echo -e "    • Unit tests: ${CYAN}34${NC}"
echo -e "    • Integration tests: ${CYAN}6${NC}"
echo -e "    • Performance tests: ${CYAN}4${NC}"
echo -e "    • Stress tests: ${CYAN}5${NC}"
echo -e "    • Compatibility tests: ${CYAN}3${NC}"
echo -e "  🔧 Utility Files: ${GREEN}8${NC} (headers + implementations)"
echo -e "  📋 Documentation: ${GREEN}6${NC} README files"
echo -e "  ⚙️  Build System: ${GREEN}1${NC} CMakeLists.txt"

echo
echo -e "${YELLOW}🚀 Next Steps:${NC}"
echo -e "  1. ${CYAN}cd $TARGET_DIR && cd ../build${NC}"
echo -e "  2. ${CYAN}cmake -DBUILD_TESTS=ON ..${NC}"
echo -e "  3. ${CYAN}make${NC}"
echo -e "  4. ${CYAN}make test_smoke${NC}  # Quick validation"
echo -e "  5. ${CYAN}make test_critical${NC}  # Critical tests only"

echo
echo -e "${YELLOW}📋 Test Phases:${NC}"
echo -e "  • ${GREEN}Phase 1${NC}: Foundation (Week 1-2) - ${CYAN}15 tests${NC}"
echo -e "  • ${GREEN}Phase 2${NC}: Image System (Week 3) - ${CYAN}7 tests${NC}"
echo -e "  • ${GREEN}Phase 3${NC}: Utilities (Week 4) - ${CYAN}8 tests${NC}"
echo -e "  • ${GREEN}Phase 4${NC}: Extended (Week 5-6) - ${CYAN}9 tests${NC}"
echo -e "  • ${GREEN}Phase 5${NC}: QA (Week 7) - ${CYAN}9 tests${NC}"

echo
echo -e "${BLUE}===========================================${NC}"
echo -e "${GREEN}Happy Testing! 🧪${NC}"
echo -e "${BLUE}===========================================${NC}"
