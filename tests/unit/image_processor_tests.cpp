// tests/unit/image_processor_tests.cpp
// Priority: ⭐ CRITICAL
// Unit tests for ImageProcessor

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

using namespace DIPAL;

/**
 * @brief Test fixture for ImageProcessor
 */
class ImageProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(ImageProcessorTest, DefaultConstruction) {
    ImageProcessor processor;
    EXPECT_FALSE(processor.canUndo());
    EXPECT_EQ(processor.getUndoCount(), 0u);
}

TEST_F(ImageProcessorTest, ApplyFilterSuccess) {
    ImageProcessor processor;
    auto imgResult = ImageFactory::createGrayscale(30, 30);
    ASSERT_TRUE(imgResult);
    for (int y = 0; y < 30; ++y)
        for (int x = 0; x < 30; ++x)
            imgResult.value()->setPixel(x, y, 100);

    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*imgResult.value(), filter);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value()->getWidth(), 30);
    EXPECT_EQ(result.value()->getHeight(), 30);
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(ImageProcessorTest, AddAndRemoveObserver) {
    ImageProcessor processor;
    auto observer = std::make_shared<ConsoleObserver>();
    processor.addObserver(observer);
    processor.removeObserver(observer);
    SUCCEED();
}

TEST_F(ImageProcessorTest, ApplyFilterNotifyesObservers) {
    class CountingObserver : public ProcessingObserver {
    public:
        int startCount = 0, completeCount = 0;
        void onProcessingStarted(std::string_view) override { ++startCount; }
        void onProgressUpdated(float) override {}
        void onProcessingCompleted(std::string_view, bool) override { ++completeCount; }
        void onError(std::string_view) override {}
    };

    auto counter = std::make_shared<CountingObserver>();
    ImageProcessor processor;
    processor.addObserver(counter);

    auto img = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(img);
    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);

    EXPECT_EQ(counter->startCount, 1);
    EXPECT_EQ(counter->completeCount, 1);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(ImageProcessorTest, MultipleObservers) {
    class CountingObserver : public ProcessingObserver {
    public:
        int count = 0;
        void onProcessingStarted(std::string_view) override { ++count; }
        void onProgressUpdated(float) override {}
        void onProcessingCompleted(std::string_view, bool) override { ++count; }
        void onError(std::string_view) override {}
    };

    auto obs1 = std::make_shared<CountingObserver>();
    auto obs2 = std::make_shared<CountingObserver>();
    ImageProcessor processor;
    processor.addObserver(obs1);
    processor.addObserver(obs2);

    auto img = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(img);
    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);

    EXPECT_EQ(obs1->count, 2);  // started + completed
    EXPECT_EQ(obs2->count, 2);
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(ImageProcessorTest, BasicPerformance) {
    ImageProcessor processor;
    auto img = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(img);
    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(ImageProcessorTest, Integration) {
    ImageProcessor processor;
    auto observer = std::make_shared<ConsoleObserver>();
    processor.addObserver(observer);

    auto img = ImageFactory::createGrayscale(15, 15);
    ASSERT_TRUE(img);
    for (int y = 0; y < 15; ++y)
        for (int x = 0; x < 15; ++x)
            img.value()->setPixel(x, y, 50);

    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);

    processor.removeObserver(observer);
    SUCCEED();
}

// Additional test cases should be added based on specific functionality
// of the class under test
