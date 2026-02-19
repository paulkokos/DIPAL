// tests/unit/error_tests.cpp
// Priority: ⭐ CRITICAL
// Unit tests for Error type

#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>

using namespace DIPAL;

/**
 * @brief Test fixture for Error
 */
class ErrorTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(ErrorTest, ConstructWithCode) {
    Error e(ErrorCode::Success);
    EXPECT_EQ(e.code(), ErrorCode::Success);
    EXPECT_EQ(e.category(), ErrorCategory::None);
    EXPECT_TRUE(e.message().empty());
}

TEST_F(ErrorTest, ConstructWithCodeAndMessage) {
    Error e(ErrorCode::FileNotFound, "file.bmp", ErrorCategory::IO);
    EXPECT_EQ(e.code(), ErrorCode::FileNotFound);
    EXPECT_EQ(e.category(), ErrorCategory::IO);
    EXPECT_EQ(e.message(), "file.bmp");
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

TEST_F(ErrorTest, ToStringNotEmpty) {
    Error e(ErrorCode::AllocationFailed, "out of memory", ErrorCategory::Memory);
    std::string s = e.toString();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find("out of memory"), std::string::npos);
}

TEST_F(ErrorTest, ImplicitStringConversion) {
    Error e(ErrorCode::InvalidParameter, "bad", ErrorCategory::Parameter);
    std::string s = e;
    EXPECT_FALSE(s.empty());
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(ErrorTest, AllErrorCodes) {
    std::vector<ErrorCode> codes = {
        ErrorCode::Success, ErrorCode::Unknown,
        ErrorCode::FileNotFound, ErrorCode::FileAccessDenied, ErrorCode::InvalidFormat,
        ErrorCode::AllocationFailed, ErrorCode::InvalidAccess,
        ErrorCode::InvalidParameter, ErrorCode::OutOfRange,
        ErrorCode::UnsupportedFormat, ErrorCode::CorruptedData,
        ErrorCode::ProcessingFailed, ErrorCode::NotImplemented, ErrorCode::InternalError
    };
    for (auto code : codes) {
        Error e(code);
        EXPECT_FALSE(e.toString().empty()) << "Empty toString for code " << static_cast<int>(code);
    }
}

// ============================================================================
// PERFORMANCE TESTS (if applicable)
// ============================================================================

TEST_F(ErrorTest, BasicPerformance) {
    for (int i = 0; i < 100; ++i) {
        Error e(ErrorCode::Unknown, "test", ErrorCategory::Parameter);
        auto s = e.toString();
        (void)s;
    }
    SUCCEED();
}

// ============================================================================
// INTEGRATION TESTS (if applicable)
// ============================================================================

TEST_F(ErrorTest, Integration) {
    auto result = makeErrorResult<int>(ErrorCode::FileNotFound, "test.bmp", ErrorCategory::IO);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code(), ErrorCode::FileNotFound);
    EXPECT_EQ(result.error().category(), ErrorCategory::IO);
}

// Additional test cases should be added based on specific functionality
// of the class under test
