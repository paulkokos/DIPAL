// tests/performance/batch_processing_tests.cpp
// Priority: 🔧 MEDIUM
// Performance test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <chrono>
#include <vector>

using namespace DIPAL;
using namespace std::chrono;

/**
 * @brief Performance test fixture for BatchProcessingPerformance
 */
class BatchProcessingPerformanceTest : public ::testing::Test {
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

TEST_F(BatchProcessingPerformanceTest, PerformanceBenchmark) {
    // Benchmark batch image creation
    std::vector<std::unique_ptr<Image>> images;

    double ms = measureExecutionTime([&]() {
        for (int i = 0; i < 10; ++i) {
            auto imgResult = ImageFactory::createGrayscale(100, 100);
            if (imgResult) {
                images.push_back(std::move(imgResult.value()));
            }
        }
    });

    EXPECT_EQ(images.size(), 10u);
    EXPECT_LT(ms, 2000.0) << "Batch creation of 10 images took " << ms << "ms";
}

// ============================================================================
// SCALABILITY TESTS
// ============================================================================

TEST_F(BatchProcessingPerformanceTest, ScalabilityTest) {
    // Test performance with increasing load
    std::vector<int> sizes = {10, 50, 100};

    for (int size : sizes) {
        std::vector<std::unique_ptr<Image>> images;

        double ms = measureExecutionTime([&]() {
            for (int i = 0; i < size; ++i) {
                auto imgResult = ImageFactory::createGrayscale(50, 50);
                if (imgResult) {
                    images.push_back(std::move(imgResult.value()));
                }
            }
        });

        EXPECT_EQ(static_cast<int>(images.size()), size);
        EXPECT_LT(ms, 10000.0) << "Creating " << size << " images took " << ms << "ms";
    }
}

// ============================================================================
// MEMORY PERFORMANCE TESTS
// ============================================================================

TEST_F(BatchProcessingPerformanceTest, MemoryPerformance) {
    // Test rapid allocation/deallocation patterns
    double ms = measureExecutionTime([&]() {
        for (int cycle = 0; cycle < 20; ++cycle) {
            auto imgResult = ImageFactory::createGrayscale(100, 100);
            if (imgResult) {
                for (int y = 0; y < 100; ++y) {
                    for (int x = 0; x < 100; ++x) {
                        imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
                    }
                }
            }
        }
    });

    EXPECT_LT(ms, 5000.0) << "20 allocation/deallocation cycles took " << ms << "ms";
}

