// tests/performance/filter_benchmark.cpp
// Priority: 🔧 MEDIUM
// Filter benchmarking tests for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <chrono>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Performance benchmark fixture for filters
 */
class FilterBenchmarkTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    template<typename Func>
    double measureExecutionTime(Func&& func) {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        return duration.count() / 1000.0;  // milliseconds
    }
};

// ============================================================================
// PERFORMANCE BENCHMARKS
// ============================================================================

TEST_F(FilterBenchmarkTest, GaussianBlurBenchmark) {
    // Benchmark Gaussian blur filter on various image sizes
    std::vector<std::pair<int, int>> sizes = {{50, 50}, {100, 100}, {150, 150}};

    for (auto [w, h] : sizes) {
        auto imgResult = ImageFactory::createGrayscale(w, h);
        ASSERT_TRUE(imgResult);

        // Populate image with test data
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
            }
        }

        GaussianBlurFilter filter(1.0f, 3);
        double ms = measureExecutionTime([&]() {
            auto result = filter.apply(*imgResult.value());
            (void)result;
        });

        EXPECT_LT(ms, 5000.0) << "GaussianBlur on " << w << "x" << h << " took " << ms << "ms";
    }
}

TEST_F(FilterBenchmarkTest, MedianFilterBenchmark) {
    // Benchmark median filter
    auto imgResult = ImageFactory::createGrayscale(100, 100);
    ASSERT_TRUE(imgResult);

    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x * y) % 256));
        }
    }

    MedianFilter filter(3);
    double ms = measureExecutionTime([&]() {
        auto result = filter.apply(*imgResult.value());
        (void)result;
    });

    EXPECT_LT(ms, 5000.0) << "MedianFilter took " << ms << "ms";
}

TEST_F(FilterBenchmarkTest, SobelFilterBenchmark) {
    // Benchmark Sobel filter
    auto imgResult = ImageFactory::createGrayscale(100, 100);
    ASSERT_TRUE(imgResult);

    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
        }
    }

    SobelFilter filter(true);
    double ms = measureExecutionTime([&]() {
        auto result = filter.apply(*imgResult.value());
        (void)result;
    });

    EXPECT_LT(ms, 5000.0) << "SobelFilter took " << ms << "ms";
}
