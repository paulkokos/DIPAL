// src/Filters/SobelFilter.cpp
#include "../../include/DIPAL/Filters/SobelFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

SobelFilter::SobelFilter(bool normalize) : m_normalize(normalize) {}

Result<std::unique_ptr<Image>> SobelFilter::apply(const Image& image) const {
    int width = image.getWidth();
    int height = image.getHeight();
    
    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply filter to an empty image"
        );
    }
    
    try {
        // Sobel operates on grayscale images, so convert to grayscale if needed
        std::unique_ptr<GrayscaleImage> grayImage;
        
        if (image.getType() == Image::Type::Grayscale) {
            // Clone the input grayscale image
            auto clone = image.clone();
            grayImage = std::unique_ptr<GrayscaleImage>(
                static_cast<GrayscaleImage*>(clone.release())
            );
        } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            // Convert color to grayscale
            const auto& colorImage = static_cast<const ColorImage&>(image);
            auto result = ImageFactory::toGrayscale(colorImage);
            if (!result) {
                return makeErrorResult<std::unique_ptr<Image>>(
                    result.error().code(),
                    result.error().message()
                );
            }
            grayImage = std::move(result.value());
        } else {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type: {}", static_cast<int>(image.getType()))
            );
        }
        
        // Create output grayscale image
        auto resultGray = ImageFactory::createGrayscale(width, height);
        if (!resultGray) {
            return makeErrorResult<std::unique_ptr<Image>>(
                resultGray.error().code(),
                resultGray.error().message()
            );
        }
        auto& outputGray = *resultGray.value();
        
        // Sobel kernels
        const int kx[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        };
        
        const int ky[3][3] = {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}
        };
        
        // Find maximum gradient magnitude for normalization if needed
        int maxMagnitude = 0;
        std::vector<int> magnitudes(width * height, 0);
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int gx = 0;
                int gy = 0;
                
                // Apply Sobel kernels
                for (int ky_idx = -1; ky_idx <= 1; ++ky_idx) {
                    for (int kx_idx = -1; kx_idx <= 1; ++kx_idx) {
                        int nx = std::clamp(x + kx_idx, 0, width - 1);
                        int ny = std::clamp(y + ky_idx, 0, height - 1);
                        
                        auto pixelResult = grayImage->getPixel(nx, ny);
                        if (!pixelResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                pixelResult.error().code(),
                                std::format("Failed to get pixel at ({}, {}): {}", 
                                          nx, ny, pixelResult.error().message())
                            );
                        }
                        
                        uint8_t pixelValue = pixelResult.value();
                        gx += pixelValue * kx[ky_idx + 1][kx_idx + 1];
                        gy += pixelValue * ky[ky_idx + 1][kx_idx + 1];
                    }
                }
                
                // Calculate gradient magnitude
                int magnitude = static_cast<int>(std::sqrt(gx * gx + gy * gy));
                
                // Store magnitude for normalization
                magnitudes[y * width + x] = magnitude;
                maxMagnitude = std::max(maxMagnitude, magnitude);
            }
        }
        
        // Set output pixels
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int magnitude = magnitudes[y * width + x];
                
                // Normalize if required
                uint8_t outputValue;
                if (m_normalize && maxMagnitude > 0) {
                    outputValue = static_cast<uint8_t>((magnitude * 255) / maxMagnitude);
                } else {
                    outputValue = static_cast<uint8_t>(std::min(255, magnitude));
                }
                
                auto setResult = outputGray.setPixel(x, y, outputValue);
                if (!setResult) {
                    return makeErrorResult<std::unique_ptr<Image>>(
                        setResult.error().code(),
                        std::format("Failed to set pixel at ({}, {}): {}", 
                                  x, y, setResult.error().message())
                    );
                }
            }
        }
        
        // Return the result
        return makeSuccessResult<std::unique_ptr<Image>>(std::move(resultGray.value()));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("Sobel filter failed: {}", e.what())
        );
    }
}

std::string_view SobelFilter::getName() const {
    return "SobelFilter";
}

std::unique_ptr<FilterStrategy> SobelFilter::clone() const {
    return std::make_unique<SobelFilter>(m_normalize);
}

bool SobelFilter::isNormalized() const noexcept {
    return m_normalize;
}

} // namespace DIPAL
