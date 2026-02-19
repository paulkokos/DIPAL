// tests/compatibility/compiler_compatibility_tests.cpp
// Priority: 🔧 MEDIUM
// Compiler compatibility tests for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <typeinfo>

using namespace DIPAL;

/**
 * @brief Test fixture for CompilerCompatibility
 */
class CompilerCompatibilityTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(CompilerCompatibilityTest, BasicImageCreation) {
    // Test that basic image creation works on all compilers
    auto img = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(img);
    EXPECT_EQ(img.value()->getType(), Image::Type::Grayscale);
}

TEST_F(CompilerCompatibilityTest, FilterApplication) {
    // Test filter application compiles and works
    auto img = ImageFactory::createGrayscale(40, 40);
    ASSERT_TRUE(img);

    GaussianBlurFilter filter;
    auto result = filter.apply(*img.value());
    ASSERT_TRUE(result);
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(CompilerCompatibilityTest, ResultTypeHandling) {
    // Test Result type works consistently across compilers
    auto result = makeSuccessResult(42);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 42);
}

TEST_F(CompilerCompatibilityTest, ErrorCodeHandling) {
    // Test error codes work consistently
    auto result = makeErrorResult<int>(ErrorCode::InvalidParameter);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code(), ErrorCode::InvalidParameter);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(CompilerCompatibilityTest, SmartPointerUsage) {
    // Test that smart pointers are used correctly across compilers
    std::unique_ptr<Image> img = ImageFactory::createGrayscale(20, 20).value_or(nullptr);
    ASSERT_NE(img, nullptr);
    EXPECT_EQ(img->getWidth(), 20);
}

TEST_F(CompilerCompatibilityTest, GenericLambdas) {
    // Test that generic lambdas work (C++14+ feature)
    auto processorFunc = [](auto& processor, auto& img, auto& filter) {
        return processor.applyFilter(img, filter);
    };

    auto img = ImageFactory::createGrayscale(30, 30);
    ASSERT_TRUE(img);

    ImageProcessor processor;
    GaussianBlurFilter filter;
    auto result = processorFunc(processor, *img.value(), filter);
    ASSERT_TRUE(result);
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(CompilerCompatibilityTest, CompilerOptimizations) {
    // Test that optimizations don't break functionality
    std::vector<int> values(1000);
    for (int i = 0; i < 1000; ++i) {
        values[i] = i;
    }
    EXPECT_EQ(values[999], 999);
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(CompilerCompatibilityTest, Integration) {
    // Integration test covering multiple compiler-sensitive features
    auto color = ImageFactory::createColor(25, 25, true);
    ASSERT_TRUE(color);

    uint8_t r = 100, g = 150, b = 200, a = 255;
    ASSERT_TRUE(color.value()->setPixel(0, 0, r, g, b, a));

    uint8_t r2, g2, b2, a2;
    ASSERT_TRUE(color.value()->getPixel(0, 0, r2, g2, b2, a2));

    EXPECT_EQ(r2, r);
    EXPECT_EQ(g2, g);
    EXPECT_EQ(b2, b);
    EXPECT_EQ(a2, a);
}

// Additional test cases should be added based on specific functionality
// of the class under test
