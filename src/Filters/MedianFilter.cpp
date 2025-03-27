// src/Filters/MedianFilter.cpp
#include "../../include/DIPAL/Filters/MedianFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"

#include <algorithm>
#include <vector>
#include <stdexcept>
#include <format>

namespace DIPAL {

MedianFilter::MedianFilter(int kernelSize) : m_kernelSize(kernelSize) {
    // Validate kernel size
    if (kernelSize <= 0 || kernelSize % 2 == 0) {
        throw std::invalid_argument(
            std::format("Kernel size must be positive and odd, got {}", kernelSize)
        );
    }
}

Result<std::unique_ptr<Image>> MedianFilter::apply(const Image& image) const {
    // Create a result image as a clone of the input
    auto result = image.clone();
    
    int width = image.getWidth();
    int height = image.getHeight();
    int radius = m_kernelSize / 2;
    
    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply filter to an empty image"
        );
    }
    
    try {
        // Process based on image type
        if (image.getType() == Image::Type::Grayscale) {
            // Process grayscale image
            const auto& srcGray = static_cast<const GrayscaleImage&>(image);
            auto& dstGray = static_cast<GrayscaleImage&>(*result);
            
            std::vector<uint8_t> neighborhood(m_kernelSize * m_kernelSize);
            
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    // Gather neighborhood pixels
                    size_t idx = 0;
                    for (int ky = -radius; ky <= radius; ++ky) {
                        for (int kx = -radius; kx <= radius; ++kx) {
                            int nx = std::clamp(x + kx, 0, width - 1);
                            int ny = std::clamp(y + ky, 0, height - 1);
                            
                            auto pixelResult = srcGray.getPixel(nx, ny);
                            if (!pixelResult) {
                                return makeErrorResult<std::unique_ptr<Image>>(
                                    pixelResult.error().code(),
                                    std::format("Failed to get pixel at ({}, {}): {}", 
                                              nx, ny, pixelResult.error().message())
                                );
                            }
                            
                            neighborhood[idx++] = pixelResult.value();
                        }
                    }
                    
                    // Find median value
                    std::sort(neighborhood.begin(), neighborhood.end());
                    uint8_t medianValue = neighborhood[neighborhood.size() / 2];
                    
                    // Set the result pixel
                    auto setResult = dstGray.setPixel(x, y, medianValue);
                    if (!setResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            setResult.error().code(),
                            std::format("Failed to set pixel at ({}, {}): {}", 
                                      x, y, setResult.error().message())
                        );
                    }
                }
            }
        } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            // Process color image
            const auto& srcColor = static_cast<const ColorImage&>(image);
            auto& dstColor = static_cast<ColorImage&>(*result);
            
            // Process each channel separately
            std::vector<uint8_t> rValues(m_kernelSize * m_kernelSize);
            std::vector<uint8_t> gValues(m_kernelSize * m_kernelSize);
            std::vector<uint8_t> bValues(m_kernelSize * m_kernelSize);
            std::vector<uint8_t> aValues;
            
            bool hasAlpha = srcColor.hasAlpha();
            if (hasAlpha) {
                aValues.resize(m_kernelSize * m_kernelSize);
            }
            
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    // Gather neighborhood pixels
                    size_t idx = 0;
                    for (int ky = -radius; ky <= radius; ++ky) {
                        for (int kx = -radius; kx <= radius; ++kx) {
                            int nx = std::clamp(x + kx, 0, width - 1);
                            int ny = std::clamp(y + ky, 0, height - 1);
                            
                            uint8_t r, g, b, a;
                            auto pixelResult = srcColor.getPixel(nx, ny, r, g, b, a);
                            if (!pixelResult) {
                                return makeErrorResult<std::unique_ptr<Image>>(
                                    pixelResult.error().code(),
                                    std::format("Failed to get pixel at ({}, {}): {}", 
                                              nx, ny, pixelResult.error().message())
                                );
                            }
                            
                            rValues[idx] = r;
                            gValues[idx] = g;
                            bValues[idx] = b;
                            if (hasAlpha) {
                                aValues[idx] = a;
                            }
                            idx++;
                        }
                    }
                    
                    // Find median values for each channel
                    std::sort(rValues.begin(), rValues.end());
                    std::sort(gValues.begin(), gValues.end());
                    std::sort(bValues.begin(), bValues.end());
                    
                    uint8_t medianR = rValues[rValues.size() / 2];
                    uint8_t medianG = gValues[gValues.size() / 2];
                    uint8_t medianB = bValues[gValues.size() / 2];
                    uint8_t medianA = 255;
                    
                    if (hasAlpha) {
                        std::sort(aValues.begin(), aValues.end());
                        medianA = aValues[aValues.size() / 2];
                    }
                    
                    // Set the result pixel
                    auto setResult = dstColor.setPixel(x, y, medianR, medianG, medianB, medianA);
                    if (!setResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            setResult.error().code(),
                            std::format("Failed to set pixel at ({}, {}): {}", 
                                      x, y, setResult.error().message())
                        );
                    }
                }
            }
        } else {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type: {}", static_cast<int>(image.getType()))
            );
        }
        
        return makeSuccessResult(std::move(result));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("Median filter failed: {}", e.what())
        );
    }
}

std::string_view MedianFilter::getName() const {
    return "MedianFilter";
}

std::unique_ptr<FilterStrategy> MedianFilter::clone() const {
    return std::make_unique<MedianFilter>(m_kernelSize);
}

int MedianFilter::getKernelSize() const noexcept {
    return m_kernelSize;
}

} // namespace DIPAL
