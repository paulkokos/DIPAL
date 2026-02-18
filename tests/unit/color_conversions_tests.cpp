// tests/unit/color_conversions_tests.cpp
// Priority: 🔧 MEDIUM
// Unit tests for ColorConversions

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

using namespace DIPAL;

/**
 * @brief Test fixture for ColorConversions
 */
class ColorConversionsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(ColorConversionsTest, RgbToGrayscale_Black) {
    RGB black{0, 0, 0};
    EXPECT_EQ(ColorConversions::rgbToGrayscale(black), 0);
}

TEST_F(ColorConversionsTest, RgbToGrayscale_White) {
    RGB white{255, 255, 255};
    EXPECT_EQ(ColorConversions::rgbToGrayscale(white), 255);
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(ColorConversionsTest, RgbToHsvAndBack_Red) {
    RGB red{255, 0, 0};
    HSV hsv = ColorConversions::rgbToHsv(red);
    EXPECT_NEAR(hsv.h, 0.0f, 1.0f);
    EXPECT_NEAR(hsv.s, 1.0f, 0.01f);
    EXPECT_NEAR(hsv.v, 1.0f, 0.01f);

    RGB back = ColorConversions::hsvToRgb(hsv);
    EXPECT_NEAR(back.r, 255, 2);
    EXPECT_NEAR(back.g, 0, 2);
    EXPECT_NEAR(back.b, 0, 2);
}

TEST_F(ColorConversionsTest, RgbToHsvAndBack_Green) {
    RGB green{0, 255, 0};
    RGB back = ColorConversions::hsvToRgb(ColorConversions::rgbToHsv(green));
    EXPECT_NEAR(back.r, 0, 2);
    EXPECT_NEAR(back.g, 255, 2);
    EXPECT_NEAR(back.b, 0, 2);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(ColorConversionsTest, BlendColors_FullOpaque) {
    RGBA fg{255, 0, 0, 255};
    RGBA bg{0, 255, 0, 255};
    RGBA blended = ColorConversions::blendColors(fg, bg);
    EXPECT_NEAR(blended.r, 255, 2);
    EXPECT_NEAR(blended.g, 0, 2);
}

TEST_F(ColorConversionsTest, BlendColors_FullTransparent) {
    RGBA fg{255, 0, 0, 0};
    RGBA bg{0, 255, 0, 255};
    RGBA blended = ColorConversions::blendColors(fg, bg);
    EXPECT_NEAR(blended.g, 255, 2);
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(ColorConversionsTest, BasicPerformance) {
    RGB color{128, 64, 200};
    for (int i = 0; i < 100; ++i) {
        auto gray = ColorConversions::rgbToGrayscale(color);
        (void)gray;
    }
    SUCCEED();
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(ColorConversionsTest, Integration) {
    RGB blue{0, 0, 255};
    uint8_t gray = ColorConversions::rgbToGrayscale(blue);
    EXPECT_GT(gray, 0);
    EXPECT_LT(gray, 255);
}

// Additional test cases should be added based on specific functionality
// of the class under test
