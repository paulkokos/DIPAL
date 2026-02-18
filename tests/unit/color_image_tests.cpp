// tests/unit/color_image_tests.cpp
// Priority: ⭐ CRITICAL
// Unit tests for ColorImage

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

using namespace DIPAL;

/**
 * @brief Test fixture for ColorImage
 */
class ColorImageTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(ColorImageTest, ConstructionRGB) {
    ColorImage img(10, 10, false);
    EXPECT_EQ(img.getWidth(), 10);
    EXPECT_EQ(img.getHeight(), 10);
    EXPECT_EQ(img.getType(), Image::Type::RGB);
    EXPECT_EQ(img.getChannels(), 3);
    EXPECT_FALSE(img.hasAlpha());
}

TEST_F(ColorImageTest, ConstructionRGBA) {
    ColorImage img(10, 10, true);
    EXPECT_EQ(img.getType(), Image::Type::RGBA);
    EXPECT_EQ(img.getChannels(), 4);
    EXPECT_TRUE(img.hasAlpha());
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(ColorImageTest, SetAndGetPixel) {
    ColorImage img(5, 5, false);
    ASSERT_TRUE(img.setPixel(1, 1, 100, 150, 200));
    uint8_t r, g, b, a;
    ASSERT_TRUE(img.getPixel(1, 1, r, g, b, a));
    EXPECT_EQ(r, 100);
    EXPECT_EQ(g, 150);
    EXPECT_EQ(b, 200);
    EXPECT_EQ(a, 255);
}

TEST_F(ColorImageTest, SetAndGetPixelWithAlpha) {
    ColorImage img(5, 5, true);
    ASSERT_TRUE(img.setPixel(0, 0, 10, 20, 30, 128));
    uint8_t r, g, b, a;
    ASSERT_TRUE(img.getPixel(0, 0, r, g, b, a));
    EXPECT_EQ(r, 10);
    EXPECT_EQ(g, 20);
    EXPECT_EQ(b, 30);
    EXPECT_EQ(a, 128);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(ColorImageTest, OutOfRangeReturnsError) {
    ColorImage img(5, 5, false);
    uint8_t r, g, b, a;
    auto result = img.getPixel(10, 10, r, g, b, a);
    EXPECT_FALSE(result.has_value());
}

TEST_F(ColorImageTest, CloneIsDeepCopy) {
    ColorImage img(4, 4, false);
    ASSERT_TRUE(img.setPixel(0, 0, 11, 22, 33));
    auto cloneBase = img.clone();
    ASSERT_NE(cloneBase, nullptr);
    auto& clone = dynamic_cast<ColorImage&>(*cloneBase);
    uint8_t r, g, b, a;
    ASSERT_TRUE(clone.getPixel(0, 0, r, g, b, a));
    EXPECT_EQ(r, 11);

    ASSERT_TRUE(clone.setPixel(0, 0, 99, 99, 99));
    uint8_t r2, g2, b2, a2;
    ASSERT_TRUE(img.getPixel(0, 0, r2, g2, b2, a2));
    EXPECT_EQ(r2, 11);  // Original unchanged
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(ColorImageTest, BasicPerformance) {
    ColorImage img(50, 50, false);
    for (int y = 0; y < 50; ++y) {
        for (int x = 0; x < 50; ++x) {
            ASSERT_TRUE(img.setPixel(x, y, 100, 150, 200));
        }
    }
    EXPECT_EQ(img.getWidth(), 50);
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(ColorImageTest, Integration) {
    auto gray = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(gray);
    gray.value()->setPixel(5, 5, 128);

    auto color = ImageFactory::toColor(*gray.value());
    ASSERT_TRUE(color);
    uint8_t r, g, b, a;
    ASSERT_TRUE(color.value()->getPixel(5, 5, r, g, b, a));
    EXPECT_EQ(r, 128);
    EXPECT_EQ(g, 128);
    EXPECT_EQ(b, 128);
}

// Additional test cases should be added based on specific functionality
// of the class under test
