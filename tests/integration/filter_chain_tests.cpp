// tests/integration/filter_chain_tests.cpp
// Priority: 🔧 HIGH
// Filter chaining integration tests for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <vector>

using namespace DIPAL;

/**
 * @brief Integration test fixture for FilterChaining
 */
class FilterChainingTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// COMPONENT INTERACTION TESTS
// ============================================================================

TEST_F(FilterChainingTest, ComponentInteraction) {
    // Test chaining of multiple filters together
    auto imgResult = ImageFactory::createGrayscale(100, 100);
    ASSERT_TRUE(imgResult);

    // Set some test data
    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
        }
    }

    // Apply first filter
    GaussianBlurFilter blurFilter(1.0f, 3);
    auto result1 = blurFilter.apply(*imgResult.value());
    ASSERT_TRUE(result1);

    // Apply second filter to first result
    SobelFilter sobelFilter(true);
    auto result2 = sobelFilter.apply(*result1.value());
    ASSERT_TRUE(result2);

    EXPECT_EQ(result2.value()->getWidth(), 100);
    EXPECT_EQ(result2.value()->getHeight(), 100);
}

// ============================================================================
// WORKFLOW TESTS
// ============================================================================

TEST_F(FilterChainingTest, CompleteWorkflow) {
    // Test complete filter chain workflow via ImageProcessor
    auto imgResult = ImageFactory::createGrayscale(80, 80);
    ASSERT_TRUE(imgResult);

    // Populate image with gradient
    for (int y = 0; y < 80; ++y) {
        for (int x = 0; x < 80; ++x) {
            uint8_t val = static_cast<uint8_t>((x * 256) / 80);
            imgResult.value()->setPixel(x, y, val);
        }
    }

    ImageProcessor processor;

    // Create command chain
    std::vector<std::unique_ptr<ProcessingCommand>> commands;
    commands.push_back(std::make_unique<FilterCommand>(std::make_unique<GaussianBlurFilter>(1.0f, 3)));
    commands.push_back(std::make_unique<FilterCommand>(std::make_unique<MedianFilter>(3)));

    // Process all
    auto result = processor.processAll(*imgResult.value(), std::span(commands.data(), commands.size()));
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value()->getType(), Image::Type::Grayscale);
    EXPECT_EQ(result.value()->getWidth(), 80);
    EXPECT_EQ(result.value()->getHeight(), 80);
}

// ============================================================================
// ERROR PROPAGATION TESTS
// ============================================================================

TEST_F(FilterChainingTest, ErrorPropagation) {
    // Test error handling in filter chains
    // Create an invalid image (0x0)
    auto invalidImg = ImageFactory::createGrayscale(0, 100);
    EXPECT_FALSE(invalidImg.has_value());

    // Create valid image
    auto validImg = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(validImg);

    // Apply filter to valid image should succeed
    GaussianBlurFilter filter;
    auto result = filter.apply(*validImg.value());
    ASSERT_TRUE(result);
    EXPECT_NE(result.value()->getWidth(), 0);
}
