// examples/basic/jpeg_example.cpp
#include <DIPAL/DIPAL.hpp>

#include <format>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        // Check command line arguments
        if (argc < 3) {
            std::cout << "Usage: " << argv[0] << " <input_image> <output_jpeg> [quality]\n";
            std::cout << "Example: " << argv[0] << " input.png output.jpg 95\n";
            return 1;
        }

        std::string inputFile = argv[1];
        std::string outputFile = argv[2];

        // Default quality is 90, but can be overridden via command line
        int quality = 90;
        if (argc >= 4) {
            quality = std::stoi(argv[3]);
            quality = std::clamp(quality, 0, 100);
        }

        std::cout << std::format("Loading image: {}\n", inputFile);

        // Load the image
        auto imageResult = DIPAL::ImageFactory::loadImage(inputFile);
        if (!imageResult) {
            std::cerr << std::format("Error loading image: {}\n", imageResult.error().toString());
            return 1;
        }

        auto& image = *imageResult.value();

        std::cout << std::format("Image loaded: {} x {} pixels, {} channels\n",
                                 image.getWidth(),
                                 image.getHeight(),
                                 image.getChannels());

        // Apply a Gaussian blur filter
        std::cout << "Applying Gaussian blur filter...\n";
        DIPAL::GaussianBlurFilter blurFilter(1.5f, 5);

        auto blurResult = blurFilter.apply(image);
        if (!blurResult) {
            std::cerr << std::format("Error applying blur filter: {}\n",
                                     blurResult.error().toString());
            return 1;
        }

        // Save the result as JPEG with specified quality
        std::cout << std::format("Saving result to: {} with quality {}\n", outputFile, quality);

        auto saveResult = DIPAL::ImageFactory::saveImage(*blurResult.value(), outputFile, quality);
        if (!saveResult) {
            std::cerr << std::format("Error saving image: {}\n", saveResult.error().toString());
            return 1;
        }

        std::cout << "Image processing completed successfully!\n";

        // Demonstrate loading the JPEG we just saved
        std::cout << std::format("Reloading the saved JPEG: {}\n", outputFile);

        auto reloadResult = DIPAL::ImageFactory::loadImage(outputFile);
        if (!reloadResult) {
            std::cerr << std::format("Error reloading JPEG: {}\n", reloadResult.error().toString());
            return 1;
        }

        auto& reloadedImage = *reloadResult.value();

        std::cout << std::format("JPEG loaded: {} x {} pixels, {} channels\n",
                                 reloadedImage.getWidth(),
                                 reloadedImage.getHeight(),
                                 reloadedImage.getChannels());

        // If we got here, JPEG loading and saving works correctly
        std::cout << "JPEG functionality test passed successfully!\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << std::format("Error: {}\n", e.what());
        return 1;
    }
}
