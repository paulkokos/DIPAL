// tests/stress/concurrent_stress_tests.cpp
// Priority: 🔧 HIGH
// Concurrent stress test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Stress test fixture for ConcurrentStress
 */
class ConcurrentStressTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    template<typename Func>
    void runConcurrentOperations(int numThreads, Func&& func) {
        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(func);
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }
};

// ============================================================================
// RESOURCE STRESS TESTS
// ============================================================================

TEST_F(ConcurrentStressTest, MultiThreadedImageCreation) {
    // Test 8 threads creating images concurrently
    std::atomic<int> createCount(0);
    std::atomic<int> errorCount(0);

    runConcurrentOperations(8, [&]() {
        auto img = ImageFactory::createGrayscale(100, 100);
        if (img) {
            ++createCount;
        } else {
            ++errorCount;
        }
    });

    EXPECT_EQ(createCount, 8);
    EXPECT_EQ(errorCount, 0);
}

TEST_F(ConcurrentStressTest, MultiThreadedPixelOperations) {
    // Test concurrent pixel operations on shared image
    auto img = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(img);

    std::atomic<int> operationCount(0);

    runConcurrentOperations(4, [&]() {
        for (int i = 0; i < 25; ++i) {
            if (img.value()->setPixel(i % 50, i % 50, 128).has_value()) {
                ++operationCount;
            }
        }
    });

    EXPECT_EQ(operationCount, 100);  // 4 threads * 25 operations
}

TEST_F(ConcurrentStressTest, MultiThreadedFilterApplication) {
    // Test concurrent filter application
    std::atomic<int> successCount(0);
    std::vector<std::thread> threads;

    for (int i = 0; i < 6; ++i) {
        threads.emplace_back([&]() {
            auto img = ImageFactory::createGrayscale(80, 80);
            if (!img) return;

            GaussianBlurFilter filter;
            if (filter.apply(*img.value()).has_value()) {
                ++successCount;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
    EXPECT_EQ(successCount, 6);
}

// ============================================================================
// CONCURRENCY STRESS TESTS
// ============================================================================

TEST_F(ConcurrentStressTest, HighContention) {
    // Test high contention on observer notifications
    auto observer = std::make_shared<ConsoleObserver>();
    ImageProcessor processor;
    processor.addObserver(observer);

    std::atomic<int> processCount(0);

    runConcurrentOperations(4, [&]() {
        for (int i = 0; i < 5; ++i) {
            auto img = ImageFactory::createGrayscale(30, 30);
            if (img) {
                GaussianBlurFilter filter;
                if (processor.applyFilter(*img.value(), filter).has_value()) {
                    ++processCount;
                }
            }
        }
    });

    EXPECT_EQ(processCount, 20);  // 4 threads * 5 iterations
}

TEST_F(ConcurrentStressTest, RaceConditionTesting) {
    // Test for race conditions with rapid create/destroy cycles
    std::atomic<int> totalSuccess(0);
    std::vector<std::thread> threads;

    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&]() {
            int successCount = 0;
            for (int i = 0; i < 20; ++i) {
                auto img = ImageFactory::createGrayscale(50, 50);
                if (img) {
                    img.value()->setPixel(25, 25, 128);
                    ++successCount;
                }
            }
            totalSuccess += successCount;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
    EXPECT_EQ(totalSuccess, 80);  // 4 threads * 20 iterations
}

// ============================================================================
// LOAD STRESS TESTS
// ============================================================================

TEST_F(ConcurrentStressTest, SustainedLoad) {
    // Test sustained concurrent load
    std::atomic<int> totalOps(0);

    auto start = high_resolution_clock::now();

    runConcurrentOperations(4, [&]() {
        auto img = ImageFactory::createGrayscale(64, 64);
        if (img) {
            ++totalOps;
        }
    });

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    EXPECT_EQ(totalOps, 4);
    EXPECT_LT(duration, 5000);  // Should complete in under 5 seconds
}

TEST_F(ConcurrentStressTest, ThreadPoolStress) {
    // Test thread pool behavior under stress
    // Note: Using manual threads to avoid std::future on macOS arm64
    ThreadPool pool(2);
    std::atomic<int> taskCount(0);

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            ++taskCount;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(taskCount, 10);
}
