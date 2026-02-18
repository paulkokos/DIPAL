// tests/unit/image_factory_tests.cpp
// Priority: 🔧 HIGH
// Unit tests for ImageFactory

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

using namespace DIPAL;

/**
 * @brief Test fixture for ImageFactory
 */
class ImageFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(ImageFactoryTest, CreateGrayscale) {
    auto r = ImageFactory::createGrayscale(100, 100);
    ASSERT_TRUE(r);
    EXPECT_EQ(r.value()->getWidth(), 100);
    EXPECT_EQ(r.value()->getHeight(), 100);
    EXPECT_EQ(r.value()->getType(), Image::Type::Grayscale);
}

TEST_F(ImageFactoryTest, CreateColor) {
    auto r = ImageFactory::createColor(50, 50, false);
    ASSERT_TRUE(r);
    EXPECT_EQ(r.value()->getType(), Image::Type::RGB);
    EXPECT_FALSE(r.value()->hasAlpha());
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(ImageFactoryTest, CreateColorWithAlpha) {
    auto r = ImageFactory::createColor(50, 50, true);
    ASSERT_TRUE(r);
    EXPECT_EQ(r.value()->getType(), Image::Type::RGBA);
    EXPECT_TRUE(r.value()->hasAlpha());
}

TEST_F(ImageFactoryTest, CreateBinary) {
    auto r = ImageFactory::createBinary(32, 32);
    ASSERT_TRUE(r);
    EXPECT_EQ(r.value()->getWidth(), 32);
    EXPECT_EQ(r.value()->getHeight(), 32);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(ImageFactoryTest, InvalidDimensionsReturnError) {
    auto r = ImageFactory::createGrayscale(0, 100);
    EXPECT_FALSE(r.has_value());

    auto r2 = ImageFactory::createGrayscale(100, 0);
    EXPECT_FALSE(r2.has_value());
}

TEST_F(ImageFactoryTest, ToColor) {
    auto gray = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(gray);
    gray.value()->setPixel(0, 0, 42);

    auto color = ImageFactory::toColor(*gray.value());
    ASSERT_TRUE(color);
    EXPECT_EQ(color.value()->getType(), Image::Type::RGB);
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(ImageFactoryTest, ToGrayscale) {
    auto color = ImageFactory::createColor(10, 10, false);
    ASSERT_TRUE(color);
    ASSERT_TRUE(color.value()->setPixel(0, 0, 90, 90, 90));

    auto gray = ImageFactory::toGrayscale(*color.value());
    ASSERT_TRUE(gray);
    EXPECT_EQ(gray.value()->getType(), Image::Type::Grayscale);
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(ImageFactoryTest, Integration) {
    auto gray = ImageFactory::createGrayscale(16, 16);
    ASSERT_TRUE(gray);
    EXPECT_EQ(gray.value()->getWidth(), 16);
    EXPECT_EQ(gray.value()->getHeight(), 16);
}

// Additional test cases should be added based on specific functionality
// of the class under test
