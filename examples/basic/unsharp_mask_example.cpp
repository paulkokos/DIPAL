// examples/basic/unsharp_mask_example.cpp
#include "../../include/DIPAL/DIPAL.hpp"
#include "../../include/DIPAL/Filters/UnsharpMaskFilter.hpp"

#include <format>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Check command line arguments
        if (argc < 3) {
            std::cout << "Usage: " << argv[0]
                      << " <input_image> <output_image> [amount] [radius] [threshold]\n";
            std::cout << "Example: " << argv[0] << " input.ppm output.ppm 1.5 1.0 5\n";
            return 1;
        }

        std::string inputFile = argv[1];
        std::string outputFile = argv[2];

        // Parse optional parameters or use defaults
        float amount = 1.5f;
        float radius = 1.0f;
        uint8_t threshold = 5;

        if (argc > 3)
            amount = std::stof(argv[3]);
        if (argc > 4)
            radius = std::stof(argv[4]);
        if (argc > 5)
            threshold = static_cast<uint8_t>(std::stoi(argv[5]));

        std::cout << std::format("Loading image: {}\n",inputFile);

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

        // Create and apply unsharp mask filter
        std::cout << std::format(
            "Applying unsharp mask filter (amount={}, radius={}, threshold={})...\n",
            amount,
            radius,
            threshold);

        DIPAL::UnsharpMaskFilter filter(amount, radius, threshold);
        auto result = filter.apply(image);

        if (!result) {
            std::cerr << std::format("Error applying unsharp mask filter: {}\n",
                                     result.error().toString());
            return 1;
        }

        // Save the result
        std::cout << std::format("Saving result to: {}\n", outputFile);

        auto saveResult = DIPAL::ImageFactory::saveImage(*result.value(), outputFile);
        if (!saveResult) {
            std::cerr << std::format("Error saving image: {}\n", saveResult.error().toString());
            return 1;
        }

        std::cout << "Image processing completed successfully!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << std::format("Error: {}\n", e.what());
        return 1;
    }
}
