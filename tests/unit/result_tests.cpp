// tests/unit/result_tests.cpp
// Priority: ⭐ CRITICAL
// Unit tests for Result type

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include <string>

using namespace DIPAL;

/**
 * @brief Test fixture for Result
 */
class ResultTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(ResultTest, SuccessResult) {
    auto r = makeSuccessResult(42);
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r.value(), 42);
}

TEST_F(ResultTest, ErrorResult) {
    auto r = makeErrorResult<int>(
        ErrorCode::InvalidParameter,
        "bad param",
        ErrorCategory::Parameter
    );
    ASSERT_FALSE(r.has_value());
    EXPECT_EQ(r.error().code(), ErrorCode::InvalidParameter);
    EXPECT_EQ(r.error().category(), ErrorCategory::Parameter);
    EXPECT_EQ(r.error().message(), "bad param");
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(ResultTest, VoidSuccess) {
    auto r = makeVoidSuccessResult();
    EXPECT_TRUE(r.has_value());
}

TEST_F(ResultTest, VoidError) {
    auto r = makeVoidErrorResult(ErrorCode::FileNotFound, "missing");
    ASSERT_FALSE(r.has_value());
    EXPECT_EQ(r.error().code(), ErrorCode::FileNotFound);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(ResultTest, MapSuccess) {
    auto r = makeSuccessResult(5);
    auto mapped = map<int, std::string>(r, [](const int& v) { return std::to_string(v); });
    ASSERT_TRUE(mapped);
    EXPECT_EQ(mapped.value(), "5");
}

TEST_F(ResultTest, MapError) {
    auto r = makeErrorResult<int>(ErrorCode::Unknown);
    auto mapped = map<int, std::string>(r, [](const int& v) { return std::to_string(v); });
    ASSERT_FALSE(mapped);
    EXPECT_EQ(mapped.error().code(), ErrorCode::Unknown);
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(ResultTest, ToOptionalHasValue) {
    auto r = makeSuccessResult(99);
    auto opt = toOptional(r);
    ASSERT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), 99);
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(ResultTest, ToOptionalEmpty) {
    auto r = makeErrorResult<int>(ErrorCode::Unknown);
    auto opt = toOptional(r);
    EXPECT_FALSE(opt.has_value());
}

// Additional test cases should be added based on specific functionality
// of the class under test
