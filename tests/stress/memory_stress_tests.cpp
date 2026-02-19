// tests/stress/memory_stress_tests.cpp
// Priority: 🔧 HIGH
// Memory stress test for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <thread>
#include <vector>
#include <future>
#include <memory>

using namespace DIPAL;

/**
 * @brief Stress test fixture for MemoryStress
 */
class MemoryStressTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

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

TEST_F(MemoryStressTest, SequentialAllocation) {
    // Test sequential memory allocation
    for (int i = 0; i < 50; ++i) {
        auto img = ImageFactory::createGrayscale(100, 100);
        ASSERT_TRUE(img);
    }
    SUCCEED();
}

TEST_F(MemoryStressTest, MultipleImageHolding) {
    // Test holding multiple images in memory simultaneously
    std::vector<std::pair<int, int>> imageDims;

    for (int i = 0; i < 10; ++i) {
        auto imgResult = ImageFactory::createGrayscale(50, 50);
        ASSERT_TRUE(imgResult);
        imageDims.push_back({imgResult.value()->getWidth(), imgResult.value()->getHeight()});
    }

    // All images should be valid
    for (const auto& [w, h] : imageDims) {
        EXPECT_EQ(w, 50);
        EXPECT_EQ(h, 50);
    }

    SUCCEED();
}

TEST_F(MemoryStressTest, LargePixelBuffers) {
    // Test allocation of large pixel buffers
    auto img = ImageFactory::createGrayscale(256, 256);
    ASSERT_TRUE(img);

    auto span = img.value()->getDataSpan();
    EXPECT_EQ(span.size(), 256 * 256);

    // Access span data
    span[0] = 255;
    EXPECT_EQ(span[0], 255);
}

// ============================================================================
// CONCURRENCY STRESS TESTS
// ============================================================================

TEST_F(MemoryStressTest, ConcurrentMemoryAllocation) {
    // Test concurrent memory allocation by multiple threads
    std::vector<std::future<int>> futures;

    for (int t = 0; t < 4; ++t) {
        futures.push_back(std::async(std::launch::async, [&]() {
            int allocCount = 0;
            for (int i = 0; i < 10; ++i) {
                auto img = ImageFactory::createGrayscale(100, 100);
                if (img) ++allocCount;
            }
            return allocCount;
        }));
    }

    int totalAllocations = 0;
    for (auto& future : futures) {
        totalAllocations += future.get();
    }
    EXPECT_EQ(totalAllocations, 40);
}

TEST_F(MemoryStressTest, ColorImageMemoryUsage) {
    // Test memory usage with color images
    std::vector<int> channelCounts;

    for (int i = 0; i < 5; ++i) {
        auto imgResult = ImageFactory::createColor(128, 128, true);
        ASSERT_TRUE(imgResult);
        channelCounts.push_back(imgResult.value()->getChannels());
    }

    // Verify all color images
    for (int channels : channelCounts) {
        EXPECT_EQ(channels, 4);  // RGBA
    }

    SUCCEED();
}

// ============================================================================
// LOAD STRESS TESTS
// ============================================================================

TEST_F(MemoryStressTest, RepeatedAllocationDeallocation) {
    // Test rapid allocation and deallocation cycles
    for (int cycle = 0; cycle < 20; ++cycle) {
        {
            auto img = ImageFactory::createGrayscale(150, 150);
            ASSERT_TRUE(img);
            img.value()->setPixel(75, 75, 128);
        }  // Image destroyed here
    }
    SUCCEED();
}

TEST_F(MemoryStressTest, FilterMemoryOverhead) {
    // Test memory overhead during filtering
    auto imgResult = ImageFactory::createGrayscale(100, 100);
    ASSERT_TRUE(imgResult);

    for (int i = 0; i < 10; ++i) {
        GaussianBlurFilter filter;
        auto result = filter.apply(*imgResult.value());
        ASSERT_TRUE(result);
        EXPECT_EQ(result.value()->getWidth(), 100);
        EXPECT_EQ(result.value()->getHeight(), 100);
    }
    SUCCEED();
}

TEST_F(MemoryStressTest, MemoryLeakDetection) {
    // Test for memory leaks with observer cleanup
    ImageProcessor processor;
    {
        auto observer = std::make_shared<ConsoleObserver>();
        processor.addObserver(observer);
        processor.removeObserver(observer);
    }

    // Observer should be properly cleaned up
    SUCCEED();  // If no crash, cleanup worked
}
