// tests/performance/performance_regression_tests.cpp
// Priority: 🔧 MEDIUM
// Performance test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <chrono>
#include <vector>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Performance test fixture for PerformanceRegression
 */
class PerformanceRegressionTest : public ::testing::Test {
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

TEST_F(PerformanceRegressionTest, PerformanceBenchmark) {
    // Benchmark core image operations to detect regressions
    auto imgResult = ImageFactory::createGrayscale(200, 200);
    ASSERT_TRUE(imgResult);

    // Populate image
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 200; ++x) {
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
        }
    }

    // Measure filter application
    GaussianBlurFilter filter(1.0f, 3);
    double ms = measureExecutionTime([&]() {
        auto result = filter.apply(*imgResult.value());
        (void)result;
    });

    // Set baseline threshold - should complete in reasonable time
    EXPECT_LT(ms, 3000.0) << "Performance regression detected: 200x200 filter took " << ms << "ms";
}

// ============================================================================
// SCALABILITY TESTS
// ============================================================================

TEST_F(PerformanceRegressionTest, ScalabilityTest) {
    // Test that performance scales with image size
    std::vector<int> sizes = {50, 100, 150};

    for (int sz : sizes) {
        auto imgResult = ImageFactory::createGrayscale(sz, sz);
        ASSERT_TRUE(imgResult);

        GaussianBlurFilter filter(1.0f, 3);
        double ms = measureExecutionTime([&]() {
            auto result = filter.apply(*imgResult.value());
            (void)result;
        });

        // Expect reasonable performance for each size
        EXPECT_LT(ms, 5000.0) << "Regression: " << sz << "x" << sz << " took " << ms << "ms";
    }
}

// ============================================================================
// MEMORY PERFORMANCE TESTS
// ============================================================================

TEST_F(PerformanceRegressionTest, MemoryPerformance) {
    // Test that memory operations don't regress
    std::vector<std::unique_ptr<Image>> images;

    double ms = measureExecutionTime([&]() {
        for (int i = 0; i < 5; ++i) {
            auto imgResult = ImageFactory::createGrayscale(256, 256);
            if (imgResult) {
                images.push_back(std::move(imgResult.value()));
            }
        }
    });

    EXPECT_EQ(images.size(), 5u);
    EXPECT_LT(ms, 2000.0) << "Memory regression: 5x256x256 images took " << ms << "ms";
}

