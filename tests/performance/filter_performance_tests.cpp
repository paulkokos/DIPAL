// tests/performance/filter_performance_tests.cpp
// Priority: 🔧 MEDIUM
// Performance test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <chrono>
#include <vector>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Performance test fixture for FilterPerformance
 */
class FilterPerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    template<typename Func>
    double measureExecutionTime(Func&& func) {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        return duration.count() / 1000.0;  // Return milliseconds
    }
};

// ============================================================================
// BENCHMARK TESTS
// ============================================================================

TEST_F(FilterPerformanceTest, PerformanceBenchmark) {
    auto imgResult = ImageFactory::createGrayscale(100, 100);
    ASSERT_TRUE(imgResult);
    for (int y = 0; y < 100; ++y)
        for (int x = 0; x < 100; ++x)
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));

    GaussianBlurFilter filter(1.0f, 3);
    double ms = measureExecutionTime([&]() {
        auto r = filter.apply(*imgResult.value());
        (void)r;
    });
    EXPECT_LT(ms, 500.0) << "GaussianBlur on 100x100 took " << ms << "ms";
}

// ============================================================================
// SCALABILITY TESTS
// ============================================================================

TEST_F(FilterPerformanceTest, ScalabilityTest) {
    GaussianBlurFilter filter(1.0f, 3);
    for (int sz : {50, 100}) {
        auto imgResult = ImageFactory::createGrayscale(sz, sz);
        ASSERT_TRUE(imgResult);
        double ms = measureExecutionTime([&]() {
            auto r = filter.apply(*imgResult.value());
            (void)r;
        });
        EXPECT_LT(ms, 5000.0) << "Filter on " << sz << "x" << sz << " took " << ms << "ms";
    }
}

// ============================================================================
// MEMORY PERFORMANCE TESTS
// ============================================================================

TEST_F(FilterPerformanceTest, MemoryPerformance) {
    GaussianBlurFilter filter;
    for (int i = 0; i < 10; ++i) {
        auto imgResult = ImageFactory::createGrayscale(50, 50);
        ASSERT_TRUE(imgResult);
        auto r = filter.apply(*imgResult.value());
        ASSERT_TRUE(r);
    }
    SUCCEED();
}
