// tests/integration/image_conversion_tests.cpp
// Priority: 🔧 HIGH
// Integration test for DIPAL Library

#include <DIPAL/DIPAL.hpp>

#include <memory>
#include <vector>

#include <gtest/gtest.h>

using namespace DIPAL;

/**
 * @brief Integration test fixture for ImageConversion
 */
class ImageConversionTest : public ::testing::Test {
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

TEST_F(ImageConversionTest, ComponentInteraction) {
    auto gray = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(gray);
    ASSERT_TRUE(gray.value()->setPixel(5, 5, 128));

    auto color = ImageFactory::toColor(*gray.value());
    ASSERT_TRUE(color);

    uint8_t r, g, b, a;
    ASSERT_TRUE(color.value()->getPixel(5, 5, r, g, b, a));
    EXPECT_EQ(r, 128);
    EXPECT_EQ(g, 128);
    EXPECT_EQ(b, 128);
}

TEST_F(ImageConversionTest, CompleteWorkflow) {
    auto color = ImageFactory::createColor(16, 16, false);
    ASSERT_TRUE(color);
    ASSERT_TRUE(color.value()->setPixel(0, 0, 200, 100, 50));

    auto gray = ImageFactory::toGrayscale(*color.value());
    ASSERT_TRUE(gray);
    EXPECT_EQ(gray.value()->getType(), Image::Type::Grayscale);
}

TEST_F(ImageConversionTest, ErrorPropagation) {
    auto invalid = ImageFactory::createGrayscale(0, 100);
    EXPECT_FALSE(invalid.has_value());
}
