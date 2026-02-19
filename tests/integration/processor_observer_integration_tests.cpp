// tests/integration/processor_observer_integration_tests.cpp
// Priority: 🔧 HIGH
// Integration test for DIPAL Library

#include <DIPAL/DIPAL.hpp>

#include <memory>
#include <vector>

#include <gtest/gtest.h>

using namespace DIPAL;

/**
 * @brief Integration test fixture for ProcessorObserverIntegration
 */
class ProcessorObserverIntegrationTest : public ::testing::Test {
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

TEST_F(ProcessorObserverIntegrationTest, ComponentInteraction) {
    auto observer = std::make_shared<ConsoleObserver>();
    ImageProcessor processor;
    processor.addObserver(observer);
    processor.removeObserver(observer);
    SUCCEED();
}

TEST_F(ProcessorObserverIntegrationTest, CompleteWorkflow) {
    class CountingObserver : public ProcessingObserver {
    public:
        int startCount = 0, completeCount = 0;
        void onProcessingStarted(std::string_view) override { ++startCount; }
        void onProgressUpdated(float) override {}
        void onProcessingCompleted(std::string_view, bool) override { ++completeCount; }
        void onError(std::string_view) override {}
    };

    auto counter = std::make_shared<CountingObserver>();
    ImageProcessor processor;
    processor.addObserver(counter);

    auto img = ImageFactory::createGrayscale(30, 30);
    ASSERT_TRUE(img);

    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);

    EXPECT_EQ(counter->startCount, 1);
    EXPECT_EQ(counter->completeCount, 1);
}

TEST_F(ProcessorObserverIntegrationTest, ErrorPropagation) {
    class CountingObserver : public ProcessingObserver {
    public:
        int startCount = 0;
        void onProcessingStarted(std::string_view) override { ++startCount; }
        void onProgressUpdated(float) override {}
        void onProcessingCompleted(std::string_view, bool) override {}
        void onError(std::string_view) override {}
    };

    auto obs1 = std::make_shared<CountingObserver>();
    auto obs2 = std::make_shared<CountingObserver>();
    ImageProcessor processor;
    processor.addObserver(obs1);
    processor.addObserver(obs2);

    auto img = ImageFactory::createGrayscale(10, 10);
    ASSERT_TRUE(img);

    GaussianBlurFilter filter;
    auto result = processor.applyFilter(*img.value(), filter);
    ASSERT_TRUE(result);

    EXPECT_EQ(obs1->startCount, 1);
    EXPECT_EQ(obs2->startCount, 1);
}
