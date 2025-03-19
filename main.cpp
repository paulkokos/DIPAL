#include "include/DIPAL/DIPAL.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <system_error>
#include <unordered_map>

int main() {
  // Create library instance
  auto dipal = std::make_unique<DIPAL::DIPAL>();
  // Load an image
  auto imageResult = dipal->loadImage("input.jpg");
  if (!imageResult) {
    std::cerr << imageResult.error() << std::endl;
    return 1;
  }

  // Get the image
  auto image = imageResult.value();

  // Create a proccessing pipeline
  std::vector<std::shared_ptr<DIPAL::ImageProcessingStrategy>> pipeline;

  // Add a Gaussian blur filter
  auto gaussianParams =
      std::unordered_map<std::string, std::any>{{"sigma", 1.5f}};
  pipeline.push_back(dipal->createStrategy("GaussianBlur", gaussianParams));

  // Add an edge detection filter
  pipeline.push_back(dipal->createStrategy("EdgeDetection"));

  // Process the image
  auto result = dipal->processImage(image, pipeline);
  if (!result) {
    std::cerr << "Failed to process image: " << result.error() << std::endl;
    return 1;
  }

  // Save the result
  auto saveResult = dipal->saveImage(image, "output.jpg");
  if (!saveResult) {
    std::cerr << "Failed to save image: " << saveResult.error() << std::endl;
    return 1;
  }

  return 0;
}
