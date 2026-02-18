// tests/unit/grayscale_image_tests.cpp
// Priority: ⭐ CRITICAL
// Unit tests for GrayscaleImage

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

using namespace DIPAL;

/**
 * @brief Test fixture for GrayscaleImage
 */
class GrayscaleImageTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(GrayscaleImageTest, ConstructionSetsProperties) {
    GrayscaleImage img(20, 30);
    EXPECT_EQ(img.getWidth(), 20);
    EXPECT_EQ(img.getHeight(), 30);
    EXPECT_EQ(img.getType(), Image::Type::Grayscale);
    EXPECT_EQ(img.getChannels(), 1);
    EXPECT_FALSE(img.isEmpty());
}

TEST_F(GrayscaleImageTest, SetAndGetPixel) {
    GrayscaleImage img(5, 5);
    auto setResult = img.setPixel(2, 2, 128);
    ASSERT_TRUE(setResult);
    auto getResult = img.getPixel(2, 2);
    ASSERT_TRUE(getResult);
    EXPECT_EQ(getResult.value(), 128);
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(GrayscaleImageTest, OutOfRangePixelError) {
    GrayscaleImage img(5, 5);
    auto r = img.getPixel(10, 10);
    EXPECT_FALSE(r.has_value());
}

TEST_F(GrayscaleImageTest, SetOutOfRangeError) {
    GrayscaleImage img(5, 5);
    auto r = img.setPixel(-1, 0, 0);
    EXPECT_FALSE(r.has_value());
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(GrayscaleImageTest, CloneProducesDeepCopy) {
    GrayscaleImage img(4, 4);
    img.setPixel(0, 0, 200);
    auto cloneBase = img.clone();
    ASSERT_NE(cloneBase, nullptr);
    auto& clone = dynamic_cast<GrayscaleImage&>(*cloneBase);
    auto r = clone.getPixel(0, 0);
    ASSERT_TRUE(r);
    EXPECT_EQ(r.value(), 200);

    clone.setPixel(0, 0, 50);
    EXPECT_EQ(img.getPixel(0, 0).value(), 200);  // Original unchanged
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(GrayscaleImageTest, GetRowSpan) {
    GrayscaleImage img(8, 4);
    img.setPixel(3, 1, 77);
    auto row = img.getRow(1);
    ASSERT_EQ(static_cast<int>(row.size()), 8);
    EXPECT_EQ(row[3], 77);
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(GrayscaleImageTest, DataSpan) {
    GrayscaleImage img(2, 2);
    auto span = img.getDataSpan();
    EXPECT_EQ(span.size(), 4u);  // 2x2x1 channel
}

// Additional test cases should be added based on specific functionality
// of the class under test
