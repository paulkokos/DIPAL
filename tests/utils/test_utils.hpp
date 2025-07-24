// tests/utils/test_utils.hpp
// Common test utilities header for DIPAL Library

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <vector>
#include <filesystem>
#include <random>

namespace DIPAL {
namespace TestUtils {

/**
 * @brief Get a shared random number generator for tests
 */
std::mt19937& getRandomGenerator();

/**
 * @brief Generate random data for testing
 */
std::vector<uint8_t> generateRandomData(size_t size);

/**
 * @brief Compare two images with tolerance
 */
bool compareImages(const Image& img1, const Image& img2, double tolerance = 0.01);

/**
 * @brief Get the path to test data directory
 */
std::filesystem::path getTestDataPath();

/**
 * @brief Timer for performance measurements
 */
class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    double elapsedMs() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return duration.count() / 1000.0;
    }
    
    void reset() {
        start_ = std::chrono::high_resolution_clock::now();
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_;
};

} // namespace TestUtils
} // namespace DIPAL
