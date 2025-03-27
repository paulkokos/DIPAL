// tests/unit/image_tests.cpp
#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

// Test fixture for Image tests
class ImageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup code
    }
    
    void TearDown() override {
        // Common cleanup code
    }
};

// Test image creation
TEST_F(ImageTest, CreateGrayscaleImage) {
    auto result = DIPAL::ImageFactory::createGrayscale(100, 100);
    ASSERT_TRUE(result) << "Failed to create grayscale image: " << result.error().toString();
    
    auto& image = *result.value();
    EXPECT_EQ(image.getWidth(), 100);
    EXPECT_EQ(image.getHeight(), 100);
    EXPECT_EQ(image.getType(), DIPAL::Image::Type::Grayscale);
    EXPECT_EQ(image.getChannels(), 1);
}

// Test pixel access
TEST_F(ImageTest, GrayscalePixelAccess) {
    auto result = DIPAL::ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Test setting pixels
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto setResult = image.setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
            EXPECT_TRUE(setResult) << "Failed to set pixel at (" << x << ", " << y << "): " 
                                 << setResult.error().toString();
        }
    }
    
    // Test getting pixels
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto getResult = image.getPixel(x, y);
            ASSERT_TRUE(getResult) << "Failed to get pixel at (" << x << ", " << y << "): " 
                                 << getResult.error().toString();
            
            EXPECT_EQ(getResult.value(), static_cast<uint8_t>((x + y) % 256));
        }
    }
}

// Test color image
TEST_F(ImageTest, CreateColorImage) {
    auto result = DIPAL::ImageFactory::createColor(100, 100, false);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    EXPECT_EQ(image.getWidth(), 100);
    EXPECT_EQ(image.getHeight(), 100);
    EXPECT_EQ(image.getType(), DIPAL::Image::Type::RGB);
    EXPECT_EQ(image.getChannels(), 3);
    EXPECT_FALSE(image.hasAlpha());
}

// Test color pixel access
TEST_F(ImageTest, ColorPixelAccess) {
    auto result = DIPAL::ImageFactory::createColor(10, 10, false);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Test setting pixels
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto setResult = image.setPixel(x, y, 
                                         static_cast<uint8_t>(x % 256),
                                         static_cast<uint8_t>(y % 256),
                                         static_cast<uint8_t>((x + y) % 256));
            EXPECT_TRUE(setResult);
        }
    }
    
    // Test getting pixels
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            uint8_t r, g, b, a;
            auto getResult = image.getPixel(x, y, r, g, b, a);
            ASSERT_TRUE(getResult);
            
            EXPECT_EQ(r, static_cast<uint8_t>(x % 256));
            EXPECT_EQ(g, static_cast<uint8_t>(y % 256));
            EXPECT_EQ(b, static_cast<uint8_t>((x + y) % 256));
            EXPECT_EQ(a, 255);  // Default alpha value
        }
    }
}

// Test image cloning
TEST_F(ImageTest, CloneImage) {
    auto result = DIPAL::ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Set some pixel values
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            image.setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
        }
    }
    
    // Clone the image
    auto clone = image.clone();
    ASSERT_NE(clone, nullptr);
    
    // Verify clone has the same properties
    EXPECT_EQ(clone->getWidth(), image.getWidth());
    EXPECT_EQ(clone->getHeight(), image.getHeight());
    EXPECT_EQ(clone->getType(), image.getType());
    
    // Verify pixel values are the same
    auto& cloneGray = static_cast<DIPAL::GrayscaleImage&>(*clone);
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto originalResult = image.getPixel(x, y);
            auto cloneResult = cloneGray.getPixel(x, y);
            
            ASSERT_TRUE(originalResult);
            ASSERT_TRUE(cloneResult);
            
            EXPECT_EQ(cloneResult.value(), originalResult.value());
        }
    }
}

// Test conversion between grayscale and color
TEST_F(ImageTest, ImageConversion) {
    // Create a grayscale image
    auto grayResult = DIPAL::ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(grayResult);
    
    auto& grayImage = *grayResult.value();
    
    // Set some values
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
           grayImage.setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
        }
    }
    
    // Convert to color
    auto colorResult = DIPAL::ImageFactory::toColor(grayImage);
    ASSERT_TRUE(colorResult);
    
    auto& colorImage = *colorResult.value();
    
    // Verify conversion
    EXPECT_EQ(colorImage.getWidth(), grayImage.getWidth());
    EXPECT_EQ(colorImage.getHeight(), grayImage.getHeight());
    EXPECT_EQ(colorImage.getType(), DIPAL::Image::Type::RGB);
    
    // Verify pixel values
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto grayPixelResult = grayImage.getPixel(x, y);
            ASSERT_TRUE(grayPixelResult);
            
            uint8_t r, g, b, a;
            auto colorPixelResult = colorImage.getPixel(x, y, r, g, b, a);
            ASSERT_TRUE(colorPixelResult);
            
            uint8_t grayValue = grayPixelResult.value();
            EXPECT_EQ(r, grayValue);
            EXPECT_EQ(g, grayValue);
            EXPECT_EQ(b, grayValue);
        }
    }
    
    // Convert back to grayscale
    auto backToGrayResult = DIPAL::ImageFactory::toGrayscale(colorImage);
    ASSERT_TRUE(backToGrayResult);
    
    auto& backToGrayImage = *backToGrayResult.value();
    
    // Verify conversion back
    EXPECT_EQ(backToGrayImage.getWidth(), colorImage.getWidth());
    EXPECT_EQ(backToGrayImage.getHeight(), colorImage.getHeight());
    EXPECT_EQ(backToGrayImage.getType(), DIPAL::Image::Type::Grayscale);
    
    // Pixel values should be approximately the same
    // (allowing for small rounding differences in the conversion)
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto originalResult = grayImage.getPixel(x, y);
            auto convertedResult = backToGrayImage.getPixel(x, y);
            
            ASSERT_TRUE(originalResult);
            ASSERT_TRUE(convertedResult);
            
            // Allow for small differences due to rounding
            EXPECT_NEAR(convertedResult.value(), originalResult.value(), 1);
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
