// src/IO/PPMImageIO.cpp
#include "../../include/DIPAL/IO/PPMImageIO.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <vector>
#include <cctype>

namespace DIPAL {

Result<std::unique_ptr<Image>> PPMImageIO::load(std::string_view filename) {
    std::ifstream file(std::string(filename), std::ios::binary);
    if (!file) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::FileNotFound,
            std::format("Failed to open file: {}", filename)
        );
    }

    try {
        // Read the PPM header
        std::string magicNumber;
        file >> magicNumber;

        // Skip comments
        char c;
        file.get(c);
        while (c == '\n' || c == ' ' || c == '\r' || c == '\t') {
            file.get(c);
            if (c == '#') {
                // Skip comment line
                while (c != '\n' && file) {
                    file.get(c);
                }
                file.get(c);
            }
        }
        file.unget();  // Put back the last character

        // Read dimensions
        int width, height, maxValue;
        file >> width >> height;

        // Skip comments after width/height
        file.get(c);
        while (c == '\n' || c == ' ' || c == '\r' || c == '\t') {
            file.get(c);
            if (c == '#') {
                while (c != '\n' && file) {
                    file.get(c);
                }
                file.get(c);
            }
        }
        file.unget();

        // Read max value
        file >> maxValue;

        // Skip a single whitespace character after the max value
        file.get(c);

        // Validate header
        if (width <= 0 || height <= 0 || maxValue <= 0 || maxValue > 255) {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::InvalidFormat,
                std::format("Invalid PPM header: width={}, height={}, maxValue={}", 
                          width, height, maxValue)
            );
        }

        // Create the appropriate image type
        std::unique_ptr<Image> image;
        
        if (magicNumber == "P3" || magicNumber == "P6") {
            // Color PPM
            auto colorResult = ImageFactory::createColor(width, height);
            if (!colorResult) {
                return makeErrorResult<std::unique_ptr<Image>>(
                    colorResult.error().code(),
                    colorResult.error().message()
                );
            }
            image = std::move(colorResult.value());
            auto& colorImage = static_cast<ColorImage&>(*image);

            if (magicNumber == "P3") {
                // ASCII format
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        int r, g, b;
                        file >> r >> g >> b;
                        
                        if (file.fail()) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                ErrorCode::InvalidFormat,
                                "Failed to read pixel data"
                            );
                        }
                        
                        // Normalize values if needed
                        if (maxValue != 255) {
                            r = static_cast<int>((static_cast<float>(r) / maxValue) * 255);
                            g = static_cast<int>((static_cast<float>(g) / maxValue) * 255);
                            b = static_cast<int>((static_cast<float>(b) / maxValue) * 255);
                        }
                        
                        auto setResult = colorImage.setPixel(x, y,
                            static_cast<uint8_t>(r), 
                            static_cast<uint8_t>(g), 
                            static_cast<uint8_t>(b)
                        );

                        if (!setResult) {
                          return makeErrorResult<std::unique_ptr<Image>>(
                            setResult.error().code(),
                            std::format("Failed to set pixel at ({}, {}): {}", 
                            x, y, setResult.error().message())
                          );
}

                    }
                }
            } else if (magicNumber == "P6") {
                // Binary format
                std::vector<uint8_t> data(width * height * 3);
                file.read(reinterpret_cast<char*>(data.data()), data.size());
                
                if (file.gcount() != static_cast<std::streamsize>(data.size())) {
                    return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::InvalidFormat,
                        "Failed to read complete pixel data"
                    );
                }
                
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        size_t idx = (y * width + x) * 3;
                        uint8_t r = data[idx];
                        uint8_t g = data[idx + 1];
                        uint8_t b = data[idx + 2];
                        
                        // Normalize values if needed
                        if (maxValue != 255) {
                            r = static_cast<uint8_t>((static_cast<float>(r) / maxValue) * 255);
                            g = static_cast<uint8_t>((static_cast<float>(g) / maxValue) * 255);
                            b = static_cast<uint8_t>((static_cast<float>(b) / maxValue) * 255);
                        }
                    auto setResult = colorImage.setPixel(x, y, r, g, b);
                    if (!setResult) {
                      return makeErrorResult<std::unique_ptr<Image>>(
                          setResult.error().code(),
                          std::format("Failed to set pixel at ({}, {}): {}", 
                          x, y, setResult.error().message())
                        );
                      }
                    }
                }
            }
        } else if (magicNumber == "P2" || magicNumber == "P5") {
            // Grayscale PGM
            auto grayResult = ImageFactory::createGrayscale(width, height);
            if (!grayResult) {
                return makeErrorResult<std::unique_ptr<Image>>(
                    grayResult.error().code(),
                    grayResult.error().message()
                );
            }
            image = std::move(grayResult.value());
            auto& grayImage = static_cast<GrayscaleImage&>(*image);

            if (magicNumber == "P2") {
                // ASCII format
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        int g;
                        file >> g;
                        
                        if (file.fail()) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                ErrorCode::InvalidFormat,
                                "Failed to read pixel data"
                            );
                        }
                        
                        // Normalize values if needed
                        if (maxValue != 255) {
                            g = static_cast<int>((static_cast<float>(g) / maxValue) * 255);
                        }
                        
                        auto setResult = grayImage.setPixel(x, y, static_cast<uint8_t>(g));
                        if (!setResult) {
                          return makeErrorResult<std::unique_ptr<Image>>(
                          setResult.error().code(),
                          std::format("Failed to set pixel at ({}, {}): {}", 
                          x, y, setResult.error().message())
                      );
                    }
                  }
                }
            } else if (magicNumber == "P5") {
                // Binary format
                std::vector<uint8_t> data(width * height);
                file.read(reinterpret_cast<char*>(data.data()), data.size());
                
                if (file.gcount() != static_cast<std::streamsize>(data.size())) {
                    return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::InvalidFormat,
                        "Failed to read complete pixel data"
                    );
                }
                
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        size_t idx = y * width + x;
                        uint8_t g = data[idx];
                        
                        // Normalize values if needed
                        if (maxValue != 255) {
                            g = static_cast<uint8_t>((static_cast<float>(g) / maxValue) * 255);
                        }
                        

                        auto setResult = grayImage.setPixel(x, y, g);
                        if (!setResult) {
                          return makeErrorResult<std::unique_ptr<Image>>(
                          setResult.error().code(),
                          std::format("Failed to set pixel at ({}, {}): {}", 
                            x, y, setResult.error().message())
                          );
                      }

                    }
                }
            }
        } else {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported PPM format: {}", magicNumber)
            );
        }

        return makeSuccessResult(std::move(image));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InternalError,
            std::format("Error reading PPM file: {}", e.what())
        );
    }
}

VoidResult PPMImageIO::save(const Image& image, std::string_view filename) {
    std::ofstream file(std::string(filename), std::ios::binary);
    if (!file) {
        return makeVoidErrorResult(
            ErrorCode::FileAccessDenied,
            std::format("Failed to create file: {}", filename)
        );
    }

    try {
        int width = image.getWidth();
        int height = image.getHeight();
        
        // Determine file type based on image type
        if (image.getType() == Image::Type::Grayscale) {
            // Save as PGM (P5 - binary)
            file << "P5\n";
            file << width << " " << height << "\n";
            file << "255\n";
            
            const auto& grayImage = static_cast<const GrayscaleImage&>(image);
            
            // Write pixel data
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    auto result = grayImage.getPixel(x, y);
                    if (!result) {
                        return makeVoidErrorResult(
                            result.error().code(),
                            result.error().message()
                        );
                    }
                    
                    uint8_t pixel = result.value();
                    file.write(reinterpret_cast<const char*>(&pixel), 1);
                }
            }
        } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            // Save as PPM (P6 - binary)
            file << "P6\n";
            file << width << " " << height << "\n";
            file << "255\n";
            
            const auto& colorImage = static_cast<const ColorImage&>(image);
            
            // Write pixel data (ignore alpha)
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    uint8_t r, g, b, a;
                    auto result = colorImage.getPixel(x, y, r, g, b, a);
                    if (!result) {
                        return makeVoidErrorResult(
                            result.error().code(),
                            result.error().message()
                        );
                    }
                    
                    uint8_t pixels[3] = {r, g, b};
                    file.write(reinterpret_cast<const char*>(pixels), 3);
                }
            }
        } else {
            return makeVoidErrorResult(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type for PPM: {}", static_cast<int>(image.getType()))
            );
        }
        
        if (!file) {
            return makeVoidErrorResult(
                ErrorCode::InternalError,
                "Failed to write to file"
            );
        }
        
        return makeVoidSuccessResult();
    } catch (const std::exception& e) {
        return makeVoidErrorResult(
            ErrorCode::InternalError,
            std::format("Error writing PPM file: {}", e.what())
        );
    }
}

} // namespace DIPAL
