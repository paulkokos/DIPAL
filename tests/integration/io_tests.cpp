// tests/integration/io_tests.cpp
// Priority: 🔧 HIGH
// I/O integration tests for DIPAL Library

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <filesystem>
#include <cstdio>

using namespace DIPAL;
namespace fs = std::filesystem;

/**
 * @brief Integration test fixture for I/O operations
 */
class IOIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = fs::temp_directory_path() / "dipal_io_tests";
        fs::create_directories(testDir);
    }

    void TearDown() override {
        if (fs::exists(testDir)) {
            fs::remove_all(testDir);
        }
    }

    fs::path testDir;
};

// ============================================================================
// COMPONENT INTERACTION TESTS
// ============================================================================

TEST_F(IOIntegrationTest, ComponentInteraction) {
    // Test ImageIO load functionality
    auto result = ImageIO::load("nonexistent.bmp");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code(), ErrorCode::FileNotFound);

    // Create a test image
    auto imgResult = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(imgResult);
    imgResult.value()->setPixel(5, 5, 128);

    // Save to temporary location
    auto testFile = testDir / "test_image.bmp";
    auto saveResult = ImageIO::save(*imgResult.value(), testFile.string());
    ASSERT_TRUE(saveResult.has_value());

    // Verify file was created
    EXPECT_TRUE(fs::exists(testFile));
}

// ============================================================================
// WORKFLOW TESTS
// ============================================================================

TEST_F(IOIntegrationTest, CompleteWorkflow) {
    // Test complete I/O workflow: create, save, load
    auto testFile = testDir / "workflow_test.bmp";

    // Create test image
    auto imgResult = ImageFactory::createGrayscale(20, 20);
    ASSERT_TRUE(imgResult);

    // Populate with data
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 20; ++x) {
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
        }
    }

    // Save image
    auto saveResult = ImageIO::save(*imgResult.value(), testFile.string());
    ASSERT_TRUE(saveResult.has_value());

    // Load image back
    auto loadResult = ImageIO::load(testFile.string());
    ASSERT_TRUE(loadResult.has_value());
    EXPECT_EQ(loadResult.value()->getWidth(), 20);
    EXPECT_EQ(loadResult.value()->getHeight(), 20);
    EXPECT_EQ(loadResult.value()->getType(), Image::Type::Grayscale);
}

// ============================================================================
// ERROR PROPAGATION TESTS
// ============================================================================

TEST_F(IOIntegrationTest, ErrorPropagation) {
    // Test error handling in I/O operations
    // Try to load from invalid path
    auto result1 = ImageIO::load("/nonexistent/path/to/file.bmp");
    EXPECT_FALSE(result1.has_value());

    // Try to save with invalid parameters
    auto imgResult = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(imgResult);
    auto result2 = ImageIO::save(*imgResult.value(), "");
    EXPECT_FALSE(result2.has_value());

    // Save to valid path should succeed
    auto validFile = testDir / "valid_save.bmp";
    auto result3 = ImageIO::save(*imgResult.value(), validFile.string());
    ASSERT_TRUE(result3.has_value());
}