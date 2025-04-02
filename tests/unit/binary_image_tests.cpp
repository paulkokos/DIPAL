// tests/unit/binary_image_tests.cpp
#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

// Test fixture for BinaryImage tests
class BinaryImageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup code
    }
    
    void TearDown() override {
        // Common cleanup code
    }
};

// Test binary image creation
TEST_F(BinaryImageTest, CreateBinaryImage) {
    auto result = DIPAL::ImageFactory::createBinary(100, 100);
    ASSERT_TRUE(result) << "Failed to create binary image: " << result.error().toString();
    
    auto& image = *result.value();
    EXPECT_EQ(image.getWidth(), 100);
    EXPECT_EQ(image.getHeight(), 100);
    EXPECT_EQ(image.getType(), DIPAL::Image::Type::Binary);
    EXPECT_EQ(image.getChannels(), 1);
    
    // Check that all pixels are initialized to black (false)
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto pixelResult = image.getPixel(x, y);
            ASSERT_TRUE(pixelResult) << "Failed to get pixel at (" << x << ", " << y << "): " 
                                  << pixelResult.error().toString();
            EXPECT_FALSE(pixelResult.value()) << "Pixel at (" << x << ", " << y << ") should be black";
        }
    }
}

// Test pixel access
TEST_F(BinaryImageTest, BinaryPixelAccess) {
    auto result = DIPAL::ImageFactory::createBinary(10, 10);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Test setting pixels
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            bool value = (x + y) % 2 == 0;  // Checkerboard pattern
            auto setResult = image.setPixel(x, y, value);
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
            
            bool expectedValue = (x + y) % 2 == 0;
            EXPECT_EQ(getResult.value(), expectedValue) 
                << "Pixel value mismatch at (" << x << ", " << y << ")";
        }
    }
}

// Test image cloning
TEST_F(BinaryImageTest, CloneBinaryImage) {
    auto result = DIPAL::ImageFactory::createBinary(10, 10);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Set checkerboard pattern
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            image.setPixel(x, y, (x + y) % 2 == 0);
        }
    }
    
    // Clone the image
    auto clonePtr = image.clone();
    ASSERT_NE(clonePtr, nullptr);
    
    // Verify clone has the same properties
    EXPECT_EQ(clonePtr->getWidth(), image.getWidth());
    EXPECT_EQ(clonePtr->getHeight(), image.getHeight());
    EXPECT_EQ(clonePtr->getType(), image.getType());
    
    // Cast to BinaryImage to access specific methods
    auto& clone = static_cast<DIPAL::BinaryImage&>(*clonePtr);
    
    // Verify pixel values are the same
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto originalResult = image.getPixel(x, y);
            auto cloneResult = clone.getPixel(x, y);
            
            ASSERT_TRUE(originalResult);
            ASSERT_TRUE(cloneResult);
            
            EXPECT_EQ(cloneResult.value(), originalResult.value())
                << "Pixel value mismatch at (" << x << ", " << y << ")";
        }
    }
}

// Test invert operation
TEST_F(BinaryImageTest, InvertBinaryImage) {
    auto result = DIPAL::ImageFactory::createBinary(10, 10);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Set checkerboard pattern
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            image.setPixel(x, y, (x + y) % 2 == 0);
        }
    }
    
    // Invert the image
    auto invertResult = image.invert();
    ASSERT_TRUE(invertResult) << "Failed to invert image: " << invertResult.error().toString();
    
    // Verify pixel values are inverted
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto pixelResult = image.getPixel(x, y);
            ASSERT_TRUE(pixelResult);
            
            bool expectedValue = (x + y) % 2 != 0;  // Inverted checkerboard
            EXPECT_EQ(pixelResult.value(), expectedValue)
                << "Pixel value mismatch at (" << x << ", " << y << ")";
        }
    }
}

// Test fill operation
TEST_F(BinaryImageTest, FillBinaryImage) {
    auto result = DIPAL::ImageFactory::createBinary(10, 10);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Set some initial pattern
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            image.setPixel(x, y, (x + y) % 2 == 0);
        }
    }
    
    // Fill with white (true)
    auto fillResult = image.fill(true);
    ASSERT_TRUE(fillResult) << "Failed to fill image: " << fillResult.error().toString();
    
    // Verify all pixels are white
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto pixelResult = image.getPixel(x, y);
            ASSERT_TRUE(pixelResult);
            EXPECT_TRUE(pixelResult.value()) << "Pixel at (" << x << ", " << y << ") should be white";
        }
    }
    
    // Fill with black (false)
    fillResult = image.fill(false);
    ASSERT_TRUE(fillResult) << "Failed to fill image: " << fillResult.error().toString();
    
    // Verify all pixels are black
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto pixelResult = image.getPixel(x, y);
            ASSERT_TRUE(pixelResult);
            EXPECT_FALSE(pixelResult.value()) << "Pixel at (" << x << ", " << y << ") should be black";
        }
    }
}

// Test white pixel counting
TEST_F(BinaryImageTest, CountWhitePixels) {
    auto result = DIPAL::ImageFactory::createBinary(10, 10);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Set checkerboard pattern (half the pixels are white)
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            image.setPixel(x, y, (x + y) % 2 == 0);
        }
    }
    
    // Count white pixels
    size_t whitePixels = image.countWhitePixels();
    EXPECT_EQ(whitePixels, 50) << "Expected 50 white pixels in a 10x10 checkerboard pattern";
    
    // Fill all with white
    image.fill(true);
    whitePixels = image.countWhitePixels();
    EXPECT_EQ(whitePixels, 100) << "Expected 100 white pixels after filling 10x10 image with white";
    
    // Fill all with black
    image.fill(false);
    whitePixels = image.countWhitePixels();
    EXPECT_EQ(whitePixels, 0) << "Expected 0 white pixels after filling 10x10 image with black";
}

// Test conversion between grayscale and binary
TEST_F(BinaryImageTest, GrayscaleToBinaryConversion) {
    // Create a grayscale image
    auto grayResult = DIPAL::ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(grayResult);
    
    auto& grayImage = *grayResult.value();
    
    // Set a pattern (gradient)
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            uint8_t value = static_cast<uint8_t>((x + y) * 12); // 0-216 range
            grayImage.setPixel(x, y, value);
        }
    }
    
    // Convert to binary using threshold of 128
    auto binaryResult = DIPAL::ImageFactory::toBinary(grayImage, 128);
    ASSERT_TRUE(binaryResult) << "Failed to convert grayscale to binary: " 
                           << binaryResult.error().toString();
    
    auto& binaryImage = *binaryResult.value();
    
    // Check that pixels are correctly thresholded
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto grayValue = grayImage.getPixel(x, y).value();
            auto binaryValue = binaryImage.getPixel(x, y).value();
            
            bool expectedValue = (grayValue >= 128);
            EXPECT_EQ(binaryValue, expectedValue)
                << "Incorrect thresholding at (" << x << ", " << y 
                << "), gray value: " << static_cast<int>(grayValue);
        }
    }
    
    // Convert back to grayscale
    auto backToGrayResult = DIPAL::ImageFactory::fromBinary(binaryImage, 255, 0);
    ASSERT_TRUE(backToGrayResult) << "Failed to convert binary to grayscale: " 
                               << backToGrayResult.error().toString();
    
    auto& backToGrayImage = *backToGrayResult.value();
    
    // Check conversion back to grayscale
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto binaryValue = binaryImage.getPixel(x, y).value();
            auto grayValue = backToGrayImage.getPixel(x, y).value();
            
            uint8_t expectedValue = binaryValue ? 255 : 0;
            EXPECT_EQ(grayValue, expectedValue)
                << "Incorrect binary to grayscale conversion at (" << x << ", " << y << ")";
        }
    }
}

// Test edge case: Non-byte-aligned image width
TEST_F(BinaryImageTest, NonByteAlignedWidth) {
    // Create an image with width that's not a multiple of 8
    auto result = DIPAL::ImageFactory::createBinary(10, 5);
    ASSERT_TRUE(result);
    
    auto& image = *result.value();
    
    // Bytes per row should be ceil(10/8) = 2
    EXPECT_EQ(image.getBytesPerRow(), 2);
    
    // Fill with white, then count pixels
    image.fill(true);
    EXPECT_EQ(image.countWhitePixels(), 50) << "Expected 50 white pixels in a 10x5 image";
    
    // Test bit-packing by setting a specific pattern
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 10; ++x) {
            image.setPixel(x, y, x % 2 == 0); // Even columns are white, odd are black
        }
    }
    
    // Verify pattern
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto pixelResult = image.getPixel(x, y);
            ASSERT_TRUE(pixelResult);
            EXPECT_EQ(pixelResult.value(), x % 2 == 0)
                << "Pixel value mismatch at (" << x << ", " << y << ")";
        }
    }
    
    // Count should be 5 * 5 = 25 white pixels (half of total)
    EXPECT_EQ(image.countWhitePixels(), 25);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
