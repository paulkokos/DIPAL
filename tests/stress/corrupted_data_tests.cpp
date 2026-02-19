// tests/stress/corrupted_data_tests.cpp
// Priority: 🔧 HIGH
// Corrupted data handling tests for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <thread>
#include <vector>
#include <future>

using namespace DIPAL;

/**
 * @brief Stress test fixture for CorruptedData
 */
class CorruptedDataTest : public ::testing::Test {
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

TEST_F(CorruptedDataTest, InvalidDimensions) {
    // Test handling of invalid dimension parameters
    auto resultZero = ImageFactory::createGrayscale(0, 0);
    EXPECT_FALSE(resultZero.has_value());

    auto resultNegative = ImageFactory::createGrayscale(-1, 100);
    EXPECT_FALSE(resultNegative.has_value());

    auto resultMixed = ImageFactory::createGrayscale(100, -1);
    EXPECT_FALSE(resultMixed.has_value());
}

TEST_F(CorruptedDataTest, InvalidPixelCoordinates) {
    // Test handling of invalid pixel coordinates
    auto img = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(img);

    // Out of bounds write
    auto resultWrite = img.value()->setPixel(100, 100, 255);
    EXPECT_FALSE(resultWrite.has_value());

    // Out of bounds read
    auto resultRead = img.value()->getPixel(-1, -1);
    EXPECT_FALSE(resultRead.has_value());

    // Negative coordinates
    auto resultNeg = img.value()->setPixel(-5, 10, 128);
    EXPECT_FALSE(resultNeg.has_value());
}

TEST_F(CorruptedDataTest, MissingFileHandling) {
    // Test handling of missing/corrupted files
    auto result = ImageIO::load("nonexistent_file_12345.bmp");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code(), ErrorCode::FileNotFound);
}

// ============================================================================
// CONCURRENCY STRESS TESTS
// ============================================================================

TEST_F(CorruptedDataTest, ConcurrentInvalidOperations) {
    // Test concurrent invalid operations don't crash
    std::atomic<int> errorCount(0);

    runConcurrentOperations(4, [&]() {
        auto img = ImageFactory::createGrayscale(50, 50);
        if (img) {
            // Try various invalid operations
            if (!img.value()->setPixel(100, 100, 255).has_value()) {
                ++errorCount;
            }
            if (!img.value()->getPixel(-1, -1).has_value()) {
                ++errorCount;
            }
            if (!img.value()->setPixel(50, 50, 255).has_value()) {
                ++errorCount;
            }
        }
    });

    EXPECT_EQ(errorCount, 12);  // 4 threads * 3 failed operations
}

TEST_F(CorruptedDataTest, CorruptedFilterInput) {
    // Test filter behavior with edge cases
    auto img = ImageFactory::createGrayscale(1, 1);
    ASSERT_TRUE(img);

    // Try to filter a 1x1 image (edge case)
    GaussianBlurFilter filter;
    auto result = filter.apply(*img.value());
    EXPECT_TRUE(result);  // Should handle gracefully
}

// ============================================================================
// LOAD STRESS TESTS
// ============================================================================

TEST_F(CorruptedDataTest, SequentialErrorRecovery) {
    // Test recovery from sequential errors
    for (int i = 0; i < 10; ++i) {
        // Attempt invalid operation
        auto invalidImg = ImageFactory::createGrayscale(-1, -1);
        EXPECT_FALSE(invalidImg.has_value());

        // Recover with valid operation
        auto validImg = ImageFactory::createGrayscale(20, 20);
        EXPECT_TRUE(validImg.has_value());
    }
    SUCCEED();
}

TEST_F(CorruptedDataTest, MixedValidInvalidOperations) {
    // Test mixed valid and invalid operations
    auto img = ImageFactory::createGrayscale(30, 30);
    ASSERT_TRUE(img);

    // Valid operation
    ASSERT_TRUE(img.value()->setPixel(15, 15, 128));

    // Invalid operation
    auto invalidResult = img.value()->setPixel(100, 100, 200);
    EXPECT_FALSE(invalidResult.has_value());

    // Valid operation again
    auto validRead = img.value()->getPixel(15, 15);
    ASSERT_TRUE(validRead);
    EXPECT_EQ(validRead.value(), 128);
}

TEST_F(CorruptedDataTest, ErrorPropagationChain) {
    // Test error propagation through operation chains
    auto img = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(img);

    // First valid operation
    auto result1 = ImageFactory::toColor(*img.value());
    ASSERT_TRUE(result1);

    // Try invalid pixel operation on result
    uint8_t r, g, b, a;
    auto invalidOp = result1.value()->getPixel(100, 100, r, g, b, a);
    EXPECT_FALSE(invalidOp.has_value());

    // Image should still be valid
    EXPECT_EQ(result1.value()->getWidth(), 50);
}

TEST_F(CorruptedDataTest, ObserverErrorHandling) {
    // Test observer behavior during errors
    auto observer = std::make_shared<ConsoleObserver>();
    ImageProcessor processor;
    processor.addObserver(observer);

    // Perform valid operation
    auto img = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(img);

    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    EXPECT_TRUE(result);

    processor.removeObserver(observer);
    SUCCEED();
}
