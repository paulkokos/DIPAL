// examples/basic/test_filters.cpp
#include <DIPAL/DIPAL.hpp>
#include <iostream>
#include <format>
#include <chrono>

// Helper function to create a test image
std::unique_ptr<DIPAL::Image> createTestImage(int width, int height) {
    auto result = DIPAL::ImageFactory::createGrayscale(width, height);
    if (!result) {
        throw std::runtime_error(std::format("Failed to create test image: {}", 
                                          result.error().toString()));
    }
    
    auto& image = *result.value();
    
    // Create a gradient pattern
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t value = static_cast<uint8_t>((x + y) % 256);
            auto setResult = image.setPixel(x, y, value);
            if (!setResult) {
              throw std::runtime_error(std::format("Failed to set pixel at ({}, {}): {}", 
                  x, y, setResult.error().toString()));
            }
        }
    }
    
    return std::unique_ptr<DIPAL::Image>(result.value().release());
}

int main() {
    try {
        std::cout << "DIPAL Filter Test" << std::endl;
        std::cout << "-----------------" << std::endl;
        
        // Create a test image
        int width = 512;
        int height = 512;
        std::cout << std::format("Creating test image ({}x{})...\n", width, height);
        
        auto image = createTestImage(width, height);
        
        // Save the original image
        std::cout << "Saving original image...\n";
        auto saveResult = DIPAL::ImageFactory::saveImage(*image, "original.ppm");
        if (!saveResult) {
            std::cerr << std::format("Failed to save original image: {}\n", 
                                   saveResult.error().toString());
            return 1;
        }
        
        // Test Gaussian blur filter
        std::cout << "Applying Gaussian blur filter...\n";
        auto startTime = std::chrono::high_resolution_clock::now();
        
        DIPAL::GaussianBlurFilter blurFilter(2.0f, 5);
        auto blurResult = blurFilter.apply(*image);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        if (!blurResult) {
            std::cerr << std::format("Failed to apply Gaussian blur: {}\n", 
                                   blurResult.error().toString());
            return 1;
        }
        
        std::cout << std::format("Gaussian blur completed in {} ms\n", duration.count());
        
        // Save the blurred image
        std::cout << "Saving blurred image...\n";
        saveResult = DIPAL::ImageFactory::saveImage(*blurResult.value(), "blurred.ppm");
        if (!saveResult) {
            std::cerr << std::format("Failed to save blurred image: {}\n", 
                                   saveResult.error().toString());
            return 1;
        }
        
        // Test Sobel filter
        std::cout << "Applying Sobel edge detection filter...\n";
        startTime = std::chrono::high_resolution_clock::now();
        
        DIPAL::SobelFilter sobelFilter(true);
        auto sobelResult = sobelFilter.apply(*blurResult.value());
        
        endTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        if (!sobelResult) {
            std::cerr << std::format("Failed to apply Sobel filter: {}\n", 
                                   sobelResult.error().toString());
            return 1;
        }
        
        std::cout << std::format("Sobel filter completed in {} ms\n", duration.count());
        
        // Save the edge-detected image
        std::cout << "Saving edge-detected image...\n";
        saveResult = DIPAL::ImageFactory::saveImage(*sobelResult.value(), "edges.ppm");
        if (!saveResult) {
            std::cerr << std::format("Failed to save edge-detected image: {}\n", 
                                   saveResult.error().toString());
            return 1;
        }
        
        // Test resize transform
        std::cout << "Applying resize transform...\n";
        startTime = std::chrono::high_resolution_clock::now();
        
        DIPAL::ResizeTransform resizeTransform(width / 2, height / 2, 
                                             DIPAL::InterpolationMethod::Bilinear);
        auto resizeResult = resizeTransform.apply(*image);
        
        endTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        if (!resizeResult) {
            std::cerr << std::format("Failed to apply resize transform: {}\n", 
                                   resizeResult.error().toString());
            return 1;
        }
        
        std::cout << std::format("Resize transform completed in {} ms\n", duration.count());
        
        // Save the resized image
        std::cout << "Saving resized image...\n";
        saveResult = DIPAL::ImageFactory::saveImage(*resizeResult.value(), "resized.ppm");
        if (!saveResult) {
            std::cerr << std::format("Failed to save resized image: {}\n", 
                                   saveResult.error().toString());
            return 1;
        }
        
        std::cout << "All tests completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << std::format("Error: {}\n", e.what());
        return 1;
    }
}
