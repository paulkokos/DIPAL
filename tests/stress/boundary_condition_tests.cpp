// tests/stress/boundary_condition_tests.cpp
// Priority: 🔧 HIGH
// Boundary condition stress tests for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <thread>
#include <vector>
#include <limits>

using namespace DIPAL;

/**
 * @brief Stress test fixture for BoundaryCondition
 */
class BoundaryConditionTest : public ::testing::Test {
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

TEST_F(BoundaryConditionTest, ZeroDimension) {
    // Test invalid 0x100 dimension
    auto imgWide = ImageFactory::createGrayscale(0, 100);
    EXPECT_FALSE(imgWide.has_value());

    // Test invalid 100x0 dimension
    auto imgTall = ImageFactory::createGrayscale(100, 0);
    EXPECT_FALSE(imgTall.has_value());

    // Test invalid 0x0 dimension
    auto imgZero = ImageFactory::createGrayscale(0, 0);
    EXPECT_FALSE(imgZero.has_value());
}

TEST_F(BoundaryConditionTest, MaxUint8Values) {
    // Test pixel operations with maximum uint8_t value
    auto img = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(img);

    uint8_t maxVal = std::numeric_limits<uint8_t>::max();
    ASSERT_TRUE(img.value()->setPixel(5, 5, maxVal));

    auto pixel = img.value()->getPixel(5, 5);
    ASSERT_TRUE(pixel);
    EXPECT_EQ(pixel.value(), maxVal);
}

TEST_F(BoundaryConditionTest, NegativePixelCoordinates) {
    // Test that negative coordinates are rejected
    auto img = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(img);

    auto result = img.value()->setPixel(-1, 5, 100);
    EXPECT_FALSE(result.has_value());

    auto result2 = img.value()->setPixel(5, -1, 100);
    EXPECT_FALSE(result2.has_value());
}

// ============================================================================
// CONCURRENCY STRESS TESTS
// ============================================================================

TEST_F(BoundaryConditionTest, ConcurrentBoundaryAccess) {
    // Test concurrent access to boundary pixels
    auto img = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(img);

    std::atomic<int> successCount(0);

    runConcurrentOperations(4, [&]() {
        // Try to access corners
        for (int i = 0; i < 10; ++i) {
            if (img.value()->getPixel(0, 0).has_value()) ++successCount;
            if (img.value()->getPixel(49, 49).has_value()) ++successCount;
        }
    });

    EXPECT_EQ(successCount, 80);  // 4 threads * 10 iterations * 2 corners
}

TEST_F(BoundaryConditionTest, OutOfBoundsAccess) {
    // Test out-of-bounds pixel access
    auto img = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(img);

    auto result1 = img.value()->getPixel(20, 10);
    EXPECT_FALSE(result1.has_value());

    auto result2 = img.value()->getPixel(10, 20);
    EXPECT_FALSE(result2.has_value());

    auto result3 = img.value()->getPixel(100, 100);
    EXPECT_FALSE(result3.has_value());
}

// ============================================================================
// LOAD STRESS TESTS
// ============================================================================

TEST_F(BoundaryConditionTest, ExtremeDimensionRatios) {
    // Test images with extreme aspect ratios
    auto wideImg = ImageFactory::createGrayscale(1000, 1);
    ASSERT_TRUE(wideImg);
    EXPECT_EQ(wideImg.value()->getWidth(), 1000);
    EXPECT_EQ(wideImg.value()->getHeight(), 1);

    auto tallImg = ImageFactory::createGrayscale(1, 1000);
    ASSERT_TRUE(tallImg);
    EXPECT_EQ(tallImg.value()->getWidth(), 1);
    EXPECT_EQ(tallImg.value()->getHeight(), 1000);
}

TEST_F(BoundaryConditionTest, EdgePixelOperations) {
    // Test operations on all edge pixels
    auto img = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(img);

    // Set all corners
    ASSERT_TRUE(img.value()->setPixel(0, 0, 1));
    ASSERT_TRUE(img.value()->setPixel(9, 0, 2));
    ASSERT_TRUE(img.value()->setPixel(0, 9, 3));
    ASSERT_TRUE(img.value()->setPixel(9, 9, 4));

    // Verify all corners
    ASSERT_EQ(img.value()->getPixel(0, 0).value(), 1);
    ASSERT_EQ(img.value()->getPixel(9, 0).value(), 2);
    ASSERT_EQ(img.value()->getPixel(0, 9).value(), 3);
    ASSERT_EQ(img.value()->getPixel(9, 9).value(), 4);
}

TEST_F(BoundaryConditionTest, SequentialBoundaryStress) {
    // Test sequential boundary operations
    for (int size = 1; size <= 50; size += 10) {
        auto img = ImageFactory::createGrayscale(size, size);
        ASSERT_TRUE(img);

        // Access all four corners
        ASSERT_TRUE(img.value()->getPixel(0, 0).has_value());
        ASSERT_TRUE(img.value()->getPixel(size - 1, 0).has_value());
        ASSERT_TRUE(img.value()->getPixel(0, size - 1).has_value());
        ASSERT_TRUE(img.value()->getPixel(size - 1, size - 1).has_value());
    }
    SUCCEED();
}
