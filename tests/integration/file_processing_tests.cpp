// tests/integration/file_processing_tests.cpp
// Priority: 🔧 HIGH
// Integration test for DIPAL Library

#include <DIPAL/DIPAL.hpp>

#include <memory>
#include <vector>
#include <filesystem>

#include <gtest/gtest.h>

namespace fs = std::filesystem;

using namespace DIPAL;

/**
 * @brief Integration test fixture for FileProcessing
 */
class FileProcessingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup for integration tests
        // Initialize multiple components that will interact
    }

    void TearDown() override {
        // Cleanup after integration tests
    }

    // Helper methods for creating test scenarios
    std::unique_ptr<Image> createTestImage(int width = 100, int height = 100) {
        auto result = ImageFactory::createGrayscale(width, height);
        if (!result) return nullptr;
        return std::move(result.value());
    }

    fs::path testDir = fs::temp_directory_path() / "dipal_tests";
};

// ============================================================================
// COMPONENT INTERACTION TESTS
// ============================================================================

TEST_F(FileProcessingTest, ComponentInteraction) {
    // Test interaction between multiple components
    auto imgResult = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(imgResult);

    // Apply a filter
    GaussianBlurFilter filter;
    auto filterResult = filter.apply(*imgResult.value());
    ASSERT_TRUE(filterResult);

    // Save result
    auto testFile = testDir / "component_test.bmp";
    fs::create_directories(testDir);
    auto saveResult = ImageIO::save(*filterResult.value(), testFile.string());
    ASSERT_TRUE(saveResult.has_value());

    // Load and verify
    auto loadResult = ImageIO::load(testFile.string());
    ASSERT_TRUE(loadResult.has_value());
    EXPECT_EQ(loadResult.value()->getWidth(), 50);

    fs::remove_all(testDir);
}

// ============================================================================
// WORKFLOW TESTS
// ============================================================================

TEST_F(FileProcessingTest, CompleteWorkflow) {
    // Test complete workflow: create image, process, save, load, process again
    fs::create_directories(testDir);

    // Create test image
    auto imgResult = ImageFactory::createGrayscale(40, 40);
    ASSERT_TRUE(imgResult);

    // Populate with data
    for (int y = 0; y < 40; ++y) {
        for (int x = 0; x < 40; ++x) {
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x * y) % 256));
        }
    }

    // Save original
    auto originalFile = testDir / "original.bmp";
    auto saveResult = ImageIO::save(*imgResult.value(), originalFile.string());
    ASSERT_TRUE(saveResult.has_value());

    // Load and process
    auto loadResult = ImageIO::load(originalFile.string());
    ASSERT_TRUE(loadResult.has_value());

    MedianFilter filter(3);
    auto processResult = filter.apply(*loadResult.value());
    ASSERT_TRUE(processResult);

    // Save processed
    auto processedFile = testDir / "processed.bmp";
    auto saveResult2 = ImageIO::save(*processResult.value(), processedFile.string());
    ASSERT_TRUE(saveResult2.has_value());

    // Verify both files exist
    EXPECT_TRUE(fs::exists(originalFile));
    EXPECT_TRUE(fs::exists(processedFile));

    fs::remove_all(testDir);
}

// ============================================================================
// ERROR PROPAGATION TESTS
// ============================================================================

TEST_F(FileProcessingTest, ErrorPropagation) {
    // Test how errors propagate through the system
    fs::create_directories(testDir);

    // Create valid image
    auto imgResult = ImageFactory::createGrayscale(30, 30);
    ASSERT_TRUE(imgResult);

    // Try to save to invalid location (should fail gracefully)
    auto result1 = ImageIO::save(*imgResult.value(), "/invalid/path/file.bmp");
    EXPECT_FALSE(result1.has_value());

    // Try to load non-existent file
    auto result2 = ImageIO::load((testDir / "nonexistent.bmp").string());
    EXPECT_FALSE(result2.has_value());

    // Valid save should work
    auto testFile = testDir / "error_test.bmp";
    auto result3 = ImageIO::save(*imgResult.value(), testFile.string());
    ASSERT_TRUE(result3.has_value());

    fs::remove_all(testDir);
}
