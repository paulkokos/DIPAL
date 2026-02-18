// tests/performance/parallel_benchmark.cpp
// Priority: 🔧 HIGH
// Parallel processing benchmark for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <chrono>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Performance test fixture for ParallelBenchmark
 */
class ParallelBenchmarkTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    template<typename Func>
    double measureExecutionTime(Func&& func) {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();
        return duration_cast<microseconds>(end - start).count() / 1000.0;
    }
};

// ============================================================================
// BENCHMARK TESTS
// ============================================================================

TEST_F(ParallelBenchmarkTest, ParallelProcessorConstruction) {
    ParallelProcessor processor(0);
    EXPECT_GT(processor.getThreadCount(), 0u);
}

// ============================================================================
// SCALABILITY TESTS
// ============================================================================

TEST_F(ParallelBenchmarkTest, ParallelFilterApply) {
    auto imgResult = ImageFactory::createGrayscale(200, 200);
    ASSERT_TRUE(imgResult);
    GaussianBlurFilter filter;

    ParallelProcessor parallel;
    double ms = measureExecutionTime([&]() {
        auto r = parallel.applyFilter(*imgResult.value(), filter);
        (void)r;
    });
    EXPECT_LT(ms, 5000.0) << "Parallel filter took " << ms << "ms";
}

// ============================================================================
// MEMORY PERFORMANCE TESTS
// ============================================================================

TEST_F(ParallelBenchmarkTest, ThreadPoolSubmitAndWait) {
    ThreadPool pool(2);
    std::atomic<int> counter{0};
    auto f1 = pool.submit([&]() { counter.fetch_add(1); });
    auto f2 = pool.submit([&]() { counter.fetch_add(1); });
    f1.get();
    f2.get();
    EXPECT_EQ(counter.load(), 2);
}
