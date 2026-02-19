// tests/unit/gaussian_blur_filter_tests.cpp
// Priority: 🔧 HIGH
// Unit tests for GaussianBlurFilter

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

using namespace DIPAL;

/**
 * @brief Test fixture for GaussianBlurFilter
 */
class GaussianBlurFilterTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(GaussianBlurFilterTest, DefaultParameters) {
    GaussianBlurFilter filter;
    EXPECT_FLOAT_EQ(filter.getSigma(), 1.0f);
    EXPECT_EQ(filter.getKernelSize(), 3);
}

TEST_F(GaussianBlurFilterTest, CustomParameters) {
    GaussianBlurFilter filter(2.0f, 5);
    EXPECT_FLOAT_EQ(filter.getSigma(), 2.0f);
    EXPECT_EQ(filter.getKernelSize(), 5);
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(GaussianBlurFilterTest, GetName) {
    GaussianBlurFilter filter;
    EXPECT_EQ(filter.getName(), "GaussianBlur");
}

TEST_F(GaussianBlurFilterTest, KernelSumIsOne) {
    GaussianBlurFilter filter(1.0f, 3);
    auto kernel = filter.getKernel();
    EXPECT_GT(kernel.size(), 0u);
    float sum = 0.0f;
    for (float v : kernel) sum += v;
    EXPECT_NEAR(sum, 1.0f, 0.001f);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(GaussianBlurFilterTest, ApplyToGrayscale) {
    auto imgResult = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(imgResult);
    for (int y = 0; y < 20; ++y)
        for (int x = 0; x < 20; ++x)
            imgResult.value()->setPixel(x, y, 128);

    GaussianBlurFilter filter;
    auto result = filter.apply(*imgResult.value());
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value()->getWidth(), 20);
    EXPECT_EQ(result.value()->getHeight(), 20);
}

TEST_F(GaussianBlurFilterTest, CloneProducesEquivalentFilter) {
    GaussianBlurFilter filter(1.5f, 5);
    auto clone = filter.clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_EQ(clone->getName(), filter.getName());
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(GaussianBlurFilterTest, BasicPerformance) {
    auto imgResult = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(imgResult);
    GaussianBlurFilter filter(1.0f, 3);
    auto result = filter.apply(*imgResult.value());
    ASSERT_TRUE(result);
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(GaussianBlurFilterTest, Integration) {
    auto img = ImageFactory::createGrayscale(30, 30);
    ASSERT_TRUE(img);
    for (int y = 0; y < 30; ++y)
        for (int x = 0; x < 30; ++x)
            img.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));

    GaussianBlurFilter filter;
    auto result = filter.apply(*img.value());
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value()->getWidth(), 30);
    EXPECT_EQ(result.value()->getHeight(), 30);
}

// Additional test cases should be added based on specific functionality
// of the class under test
