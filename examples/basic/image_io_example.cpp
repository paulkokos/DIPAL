// examples/basic/image_io_example.cpp
#include <DIPAL/DIPAL.hpp>
#include <iostream>
#include <string>
#include <format>

int main(int argc, char* argv[]) {
    // Initialize the logger
    //DIPAL::Logger::getInstance().initialize("dipal_example.log", DIPAL::LogLevel::Debug, true);
    
    // Enable profiling
    //DIPAL::Profiler::getInstance().setEnabled(true);
    
    // Profile the entire program
    //DIPAL_PROFILE_SCOPE("Main Program");
    
    try {
        // Check command line arguments
        if (argc < 3) {
            std::cout << "Usage: " << argv[0] << " <input_image> <output_image>\n";
            std::cout << "Example: " << argv[0] << " input.ppm output.bmp\n";
            return 1;
        }
        
        std::string inputFile = argv[1];
        std::string outputFile = argv[2];
        
        //DIPAL_LOG_INFO_FORMAT("Processing {} to {}", inputFile, outputFile);
        
        // Create a console observer to show progress
        auto observer = std::make_shared<DIPAL::ConsoleObserver>();
        
        // Create an image processor with the observer
        DIPAL::ImageProcessor processor;
        processor.addObserver(observer);
        
        {
            DIPAL_PROFILE_SCOPE("Image Load");
            std::cout << std::format("Loading image: {}\n", inputFile);
            
            // Load the image
            auto imageResult = DIPAL::ImageFactory::loadImage(inputFile);
            if (!imageResult) {
                //DIPAL_LOG_ERROR_FORMAT("Error loading image: {}", imageResult.error().toString());
                std::cerr << std::format("Error loading image: {}\n", imageResult.error().toString());
                return 1;
            }
            
            auto& image = *imageResult.value();
            
            std::cout << std::format("Image loaded: {} x {} pixels, {} channels\n", 
                                  image.getWidth(), image.getHeight(), image.getChannels());
            //DIPAL_LOG_INFO_FORMAT("Image loaded: {}x{}, {} channels", 
            //                   image.getWidth(), image.getHeight(), image.getChannels());
            
            // Apply some filters
            {
            //    DIPAL_PROFILE_SCOPE("Filtering");
                
                // Create a Gaussian blur filter
                std::cout << "Applying Gaussian blur filter...\n";
                auto blurFilter = std::make_unique<DIPAL::GaussianBlurFilter>(1.5f, 5);
                
                // Process the image
                auto blurResult = processor.applyFilter(image, *blurFilter);
                if (!blurResult) {
            //        DIPAL_LOG_ERROR_FORMAT("Error applying blur filter: {}", blurResult.error().toString());
                    std::cerr << std::format("Error applying blur filter: {}\n", blurResult.error().toString());
                    return 1;
                }
                
                // Apply edge detection
                std::cout << "Applying Sobel edge detection...\n";
                auto sobelFilter = std::make_unique<DIPAL::SobelFilter>(true);
                
                auto sobelResult = processor.applyFilter(*blurResult.value(), *sobelFilter);
                if (!sobelResult) {
            //        DIPAL_LOG_ERROR_FORMAT("Error applying Sobel filter: {}", sobelResult.error().toString());
                    std::cerr << std::format("Error applying Sobel filter: {}\n", sobelResult.error().toString());
                    return 1;
                }
                
                // Now resize the image
                std::cout << "Resizing image...\n";
                int newWidth = image.getWidth() / 2;
                int newHeight = image.getHeight() / 2;
                
                DIPAL::ResizeTransform resizer(newWidth, newHeight, DIPAL::InterpolationMethod::Bilinear);
                
                auto resizeResult = resizer.apply(*sobelResult.value());
                if (!resizeResult) {
            //        DIPAL_LOG_ERROR_FORMAT("Error resizing image: {}", resizeResult.error().toString());
                    std::cerr << std::format("Error resizing image: {}\n", resizeResult.error().toString());
                    return 1;
                }
                
                // Save the result
                std::cout << std::format("Saving result to: {}\n", outputFile);
                
                {
            //        DIPAL_PROFILE_SCOPE("Image Save");
                    auto saveResult = DIPAL::ImageFactory::saveImage(*resizeResult.value(), outputFile);
                    if (!saveResult) {
                        //DIPAL_LOG_ERROR_FORMAT("Error saving image: {}", saveResult.error().toString());
                        std::cerr << std::format("Error saving image: {}\n", saveResult.error().toString());
                        return 1;
                    }
                }
                
                std::cout << "Image processing completed successfully!\n";
            //    DIPAL_LOG_INFO("Image processing completed successfully");
            }
        }
        
        // Log profiling results
      //  DIPAL::Profiler::getInstance().logSummaries(DIPAL::LogLevel::Info);
        
        return 0;
    } catch (const std::exception& e) {
    //    DIPAL_LOG_ERROR_FORMAT("Unhandled exception: {}", e.what());
        std::cerr << std::format("Error: {}\n", e.what());
        return 1;
    }
}
