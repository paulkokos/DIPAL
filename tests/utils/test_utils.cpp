// tests/utils/test_utils.cpp
// Common test utilities for DIPAL Library

#include "test_utils.hpp"
#include <DIPAL/DIPAL.hpp>
#include <random>
#include <chrono>

namespace DIPAL {
namespace TestUtils {

std::mt19937& getRandomGenerator() {
    static std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    return gen;
}

std::vector<uint8_t> generateRandomData(size_t size) {
    std::vector<uint8_t> data(size);
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    auto& gen = getRandomGenerator();
    
    for (auto& byte : data) {
        byte = dist(gen);
    }
    
    return data;
}

bool compareImages(const Image& img1, const Image& img2, double tolerance) {
    if (img1.getWidth() != img2.getWidth() || 
        img1.getHeight() != img2.getHeight() ||
        img1.getType() != img2.getType()) {
        return false;
    }
    
    // TODO: Implement pixel-by-pixel comparison with tolerance
    return true;
}

std::filesystem::path getTestDataPath() {
    return std::filesystem::current_path() / "fixtures";
}

} // namespace TestUtils
} // namespace DIPAL
