// tests/integration/filter_pipeline_integration_tests.cpp
// Priority: 🔧 HIGH
// Integration test for DIPAL Library

#include <DIPAL/DIPAL.hpp>

#include <memory>
#include <vector>
#include <span>

#include <gtest/gtest.h>

using namespace DIPAL;

/**
 * @brief Integration test fixture for FilterPipelineIntegration
 */
class FilterPipelineIntegrationTest : public ::testing::Test {
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

TEST_F(FilterPipelineIntegrationTest, ComponentInteraction) {
    auto imgResult = ImageFactory::createGrayscale(50, 50);
    ASSERT_TRUE(imgResult);

    ImageProcessor processor;
    GaussianBlurFilter filter(1.0f, 3);

    auto result = processor.applyFilter(*imgResult.value(), filter);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value()->getWidth(), 50);
    EXPECT_EQ(result.value()->getHeight(), 50);
}

TEST_F(FilterPipelineIntegrationTest, CompleteWorkflow) {
    auto imgResult = ImageFactory::createGrayscale(64, 64);
    ASSERT_TRUE(imgResult);

    for (int y = 0; y < 64; ++y) {
        for (int x = 0; x < 64; ++x) {
            imgResult.value()->setPixel(x, y, static_cast<uint8_t>((x + y) % 256));
        }
    }

    ImageProcessor processor;
    std::vector<std::unique_ptr<ProcessingCommand>> commands;
    commands.push_back(std::make_unique<FilterCommand>(std::make_unique<GaussianBlurFilter>(1.0f, 3)));
    commands.push_back(std::make_unique<FilterCommand>(std::make_unique<SobelFilter>(true)));

    auto result = processor.processAll(*imgResult.value(), std::span(commands.data(), commands.size()));
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value()->getType(), Image::Type::Grayscale);
}

TEST_F(FilterPipelineIntegrationTest, ErrorPropagation) {
    auto result = ImageIO::load("nonexistent_file.bmp");
    ASSERT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().toString().empty());
    EXPECT_EQ(result.error().code(), ErrorCode::FileNotFound);
}
