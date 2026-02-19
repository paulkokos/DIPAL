// tests/stress/extreme_image_sizes_tests.cpp
// Priority: 🔧 HIGH
// Stress test for extreme image sizes

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <thread>
#include <vector>

using namespace DIPAL;

/**
 * @brief Stress test fixture for ExtremeImageSizes
 */
class ExtremeImageSizesTest : public ::testing::Test {
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

TEST_F(ExtremeImageSizesTest, MinimalImageSize) {
    // Test 1x1 pixel image (minimal size)
    auto img = ImageFactory::createGrayscale(1, 1);
    ASSERT_TRUE(img);
    EXPECT_EQ(img.value()->getWidth(), 1);
    EXPECT_EQ(img.value()->getHeight(), 1);

    ASSERT_TRUE(img.value()->setPixel(0, 0, 255));
    auto pixel = img.value()->getPixel(0, 0);
    ASSERT_TRUE(pixel);
    EXPECT_EQ(pixel.value(), 255);
}

TEST_F(ExtremeImageSizesTest, LargeImageCreation) {
    // Test large image creation (256x256 = 65,536 pixels)
    auto img = ImageFactory::createGrayscale(256, 256);
    ASSERT_TRUE(img);
    EXPECT_EQ(img.value()->getWidth(), 256);
    EXPECT_EQ(img.value()->getHeight(), 256);

    // Test pixel access at extremes
    ASSERT_TRUE(img.value()->setPixel(0, 0, 100));
    ASSERT_TRUE(img.value()->setPixel(255, 255, 200));

    auto p1 = img.value()->getPixel(0, 0);
    auto p2 = img.value()->getPixel(255, 255);
    ASSERT_TRUE(p1);
    ASSERT_TRUE(p2);
    EXPECT_EQ(p1.value(), 100);
    EXPECT_EQ(p2.value(), 200);
}

TEST_F(ExtremeImageSizesTest, VeryLargeImageProcessing) {
    // Test processing of large image (512x512)
    auto img = ImageFactory::createGrayscale(512, 512);
    ASSERT_TRUE(img);

    GaussianBlurFilter filter;
    auto result = filter.apply(*img.value());
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value()->getWidth(), 512);
    EXPECT_EQ(result.value()->getHeight(), 512);
}

// ============================================================================
// CONCURRENCY STRESS TESTS
// ============================================================================

TEST_F(ExtremeImageSizesTest, ConcurrentImageCreation) {
    // Test concurrent creation of multiple large images
    std::atomic<int> successCount(0);

    runConcurrentOperations(4, [&]() {
        auto img = ImageFactory::createGrayscale(200, 200);
        if (img) {
            ++successCount;
        }
    });

    EXPECT_EQ(successCount, 4);
}

TEST_F(ExtremeImageSizesTest, ConcurrentFilterApplication) {
    // Test concurrent filter application on large images
    std::vector<std::future<bool>> futures;

    for (int i = 0; i < 3; ++i) {
        futures.push_back(std::async(std::launch::async, [&]() {
            auto img = ImageFactory::createGrayscale(300, 300);
            if (!img) return false;

            GaussianBlurFilter filter;
            auto result = filter.apply(*img.value());
            return result.has_value();
        }));
    }

    int successCount = 0;
    for (auto& future : futures) {
        if (future.get()) ++successCount;
    }
    EXPECT_EQ(successCount, 3);
}

// ============================================================================
// LOAD STRESS TESTS
// ============================================================================

TEST_F(ExtremeImageSizesTest, SequentialLargeImageProcessing) {
    // Test sequential processing of multiple large images
    for (int i = 0; i < 5; ++i) {
        auto img = ImageFactory::createGrayscale(128, 128);
        ASSERT_TRUE(img);

        GaussianBlurFilter filter;
        auto result = filter.apply(*img.value());
        ASSERT_TRUE(result);
    }
    SUCCEED();
}

TEST_F(ExtremeImageSizesTest, ColorImageStress) {
    // Test large color image creation and processing
    auto img = ImageFactory::createColor(256, 256, true);
    ASSERT_TRUE(img);

    // Set and retrieve pixels at various locations
    for (int i = 0; i < 10; ++i) {
        int x = (i * 25) % 256;
        int y = (i * 15) % 256;
        ASSERT_TRUE(img.value()->setPixel(x, y, 100 + i, 150 - i, 200));
    }

    SUCCEED();
}
