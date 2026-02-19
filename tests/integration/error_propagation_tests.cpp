// tests/integration/error_propagation_tests.cpp
// Priority: 🔧 HIGH
// Integration test for DIPAL Library

#include <DIPAL/DIPAL.hpp>

#include <memory>
#include <vector>

#include <gtest/gtest.h>

using namespace DIPAL;

/**
 * @brief Integration test fixture for ErrorPropagation
 */
class ErrorPropagationTest : public ::testing::Test {
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

TEST_F(ErrorPropagationTest, ComponentInteraction) {
    auto result = ImageIO::load("nonexistent.bmp");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code(), ErrorCode::FileNotFound);
}

TEST_F(ErrorPropagationTest, CompleteWorkflow) {
    auto result = ImageIO::load("/tmp/does_not_exist.jpg");
    ASSERT_FALSE(result);
    EXPECT_NE(result.error().toString().find("does_not_exist"), std::string::npos);
}

TEST_F(ErrorPropagationTest, ErrorPropagation) {
    auto err = makeErrorResult<int>(
        ErrorCode::InvalidParameter, "test msg", ErrorCategory::Parameter
    );
    ASSERT_FALSE(err);
    EXPECT_EQ(err.error().code(), ErrorCode::InvalidParameter);
    EXPECT_EQ(err.error().category(), ErrorCategory::Parameter);
}
