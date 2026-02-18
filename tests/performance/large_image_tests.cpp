// tests/performance/large_image_tests.cpp
// Priority: 🔧 HIGH
// Performance test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <chrono>
#include <vector>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Performance test fixture for LargeImagePerformance
 */
class LargeImageTest : public ::testing::Test {
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

TEST_F(LargeImageTest, PerformanceBenchmark) {
    double ms = measureExecutionTime([&]() {
        auto r = ImageFactory::createGrayscale(1000, 1000);
        ASSERT_TRUE(r);
    });
    EXPECT_LT(ms, 2000.0) << "Creating 1000x1000 took " << ms << "ms";
}

// ============================================================================
// SCALABILITY TESTS
// ============================================================================

TEST_F(LargeImageTest, ScalabilityTest) {
    auto imgResult = ImageFactory::createGrayscale(512, 512);
    ASSERT_TRUE(imgResult);
    GaussianBlurFilter filter(1.0f, 5);
    double ms = measureExecutionTime([&]() {
        auto r = filter.apply(*imgResult.value());
        (void)r;
    });
    EXPECT_LT(ms, 3000.0) << "Filter on 512x512 took " << ms << "ms";
}

// ============================================================================
// MEMORY PERFORMANCE TESTS
// ============================================================================

TEST_F(LargeImageTest, MemoryPerformance) {
    auto imgResult = ImageFactory::createGrayscale(500, 500);
    ASSERT_TRUE(imgResult);
    auto& img = *imgResult.value();
    img.setPixel(499, 499, 200);
    auto r = img.getPixel(499, 499);
    ASSERT_TRUE(r);
    EXPECT_EQ(r.value(), 200);
}
