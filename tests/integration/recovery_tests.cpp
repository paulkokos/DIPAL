// tests/integration/recovery_tests.cpp
// Priority: 🔧 HIGH
// Integration test for DIPAL Library

#include <DIPAL/DIPAL.hpp>

#include <memory>
#include <vector>

#include <gtest/gtest.h>

using namespace DIPAL;

/**
 * @brief Integration test fixture for Recovery
 */
class RecoveryTest : public ::testing::Test {
protected:
    void SetUp() override {}

    void TearDown() override {}

    std::unique_ptr<Image> createTestImage(int width = 100,
                                           int height = 100) {
        auto result = ImageFactory::createGrayscale(width, height);
        if (!result) return nullptr;
        return std::move(result.value());
    }
};

TEST_F(RecoveryTest, ComponentInteraction) {
    auto img = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(img);

    ImageProcessor processor;
    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);

    auto result2 = processor.applyFilter(*result.value(), filter);
    ASSERT_TRUE(result2);
}

TEST_F(RecoveryTest, CompleteWorkflow) {
    ImageProcessor processor;
    EXPECT_FALSE(processor.canUndo());
    EXPECT_EQ(processor.getUndoCount(), 0u);

    auto img = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(img);
    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);
}

TEST_F(RecoveryTest, ErrorPropagation) {
    auto observer = std::make_shared<ConsoleObserver>();
    ImageProcessor processor;
    processor.addObserver(observer);

    auto img = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(img);

    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);

    processor.removeObserver(observer);
    SUCCEED();
}
