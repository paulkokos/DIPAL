// src/Filters/GaussianBlurFilter.cpp
#include "../../include/DIPAL/Filters/GaussianBlurFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"

#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <format>

namespace DIPAL {

GaussianBlurFilter::GaussianBlurFilter(float sigma, int kernelSize)
    : m_sigma(sigma), m_kernelSize(kernelSize) {
    // Kernel size must be odd
    if (kernelSize <= 0 || kernelSize % 2 == 0) {
        throw std::invalid_argument(std::format("Kernel size must be positive and odd, got {}", kernelSize));
    }
    
    if (sigma <= 0.0f) {
        throw std::invalid_argument(std::format("Sigma must be positive, got {}", sigma));
    }

    generateKernel();
}

Result<std::unique_ptr<Image>> GaussianBlurFilter::apply(const Image& image) const {
    // Create a result image as a clone of the input
    auto result = image.clone();
    
    // Create a temporary image for horizontal pass
    auto temp = image.clone();
    
    int width = image.getWidth();
    int height = image.getHeight();
    [[maybe_unused]] int channels = image.getChannels();
    
    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply filter to an empty image"
        );
    }
    
    int halfKernel = m_kernelSize / 2;
    
    try {
        // Optimize for grayscale vs. color images
        if (image.getType() == Image::Type::Grayscale) {
            // Cast to grayscale
            auto& grayImage = static_cast<const GrayscaleImage&>(image);
            auto& grayTemp = static_cast<GrayscaleImage&>(*temp);
            auto& grayResult = static_cast<GrayscaleImage&>(*result);
            
            // Horizontal pass
            for (int y = 0; y < height; ++y) {
                [[maybe_unused]] auto rowSpan = grayTemp.getRow(y);
                
                for (int x = 0; x < width; ++x) {
                    float sum = 0.0f;
                    
                    for (int k = -halfKernel; k <= halfKernel; ++k) {
                        int sampleX = std::clamp(x + k, 0, width - 1);
                        
                        auto pixelResult = grayImage.getPixel(sampleX, y);
                        if (!pixelResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                pixelResult.error().code(),
                                std::format("Failed to get pixel at ({}, {}): {}", 
                                          sampleX, y, pixelResult.error().message())
                            );
                        }
                        
                        int kernelIdx = k + halfKernel;
                        sum += pixelResult.value() * m_kernel[kernelIdx];
                    }
                    
                    auto setResult = grayTemp.setPixel(x, y, static_cast<uint8_t>(sum));
                    if (!setResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            setResult.error().code(),
                            std::format("Failed to set pixel at ({}, {}): {}", 
                                      x, y, setResult.error().message())
                        );
                    }
                }
            }
            
            // Vertical pass
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    float sum = 0.0f;
                    
                    for (int k = -halfKernel; k <= halfKernel; ++k) {
                        int sampleY = std::clamp(y + k, 0, height - 1);
                        
                        auto pixelResult = grayTemp.getPixel(x, sampleY);
                        if (!pixelResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                pixelResult.error().code(),
                                std::format("Failed to get pixel at ({}, {}): {}", 
                                          x, sampleY, pixelResult.error().message())
                            );
                        }
                        
                        int kernelIdx = k + halfKernel;
                        sum += pixelResult.value() * m_kernel[kernelIdx];
                    }
                    
                    auto setResult = grayResult.setPixel(x, y, static_cast<uint8_t>(sum));
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
            // Cast to color
            auto& colorImage = static_cast<const ColorImage&>(image);
            auto& colorTemp = static_cast<ColorImage&>(*temp);
            auto& colorResult = static_cast<ColorImage&>(*result);
            bool hasAlpha = colorImage.hasAlpha();
            
            // Horizontal pass
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f, sumA = 0.0f;
                    
                    for (int k = -halfKernel; k <= halfKernel; ++k) {
                        int sampleX = std::clamp(x + k, 0, width - 1);
                        
                        uint8_t r, g, b, a;
                        auto pixelResult = colorImage.getPixel(sampleX, y, r, g, b, a);
                        if (!pixelResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                pixelResult.error().code(),
                                std::format("Failed to get pixel at ({}, {}): {}", 
                                          sampleX, y, pixelResult.error().message())
                            );
                        }
                        
                        int kernelIdx = k + halfKernel;
                        sumR += r * m_kernel[kernelIdx];
                        sumG += g * m_kernel[kernelIdx];
                        sumB += b * m_kernel[kernelIdx];
                        if (hasAlpha) {
                            sumA += a * m_kernel[kernelIdx];
                        }
                    }
                    
                    auto setResult = colorTemp.setPixel(x, y, 
                        static_cast<uint8_t>(sumR), 
                        static_cast<uint8_t>(sumG), 
                        static_cast<uint8_t>(sumB), 
                        hasAlpha ? static_cast<uint8_t>(sumA) : 255
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
            
            // Vertical pass
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f, sumA = 0.0f;
                    
                    for (int k = -halfKernel; k <= halfKernel; ++k) {
                        int sampleY = std::clamp(y + k, 0, height - 1);
                        
                        uint8_t r, g, b, a;
                        auto pixelResult = colorTemp.getPixel(x, sampleY, r, g, b, a);
                        if (!pixelResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                pixelResult.error().code(),
                                std::format("Failed to get pixel at ({}, {}): {}", 
                                          x, sampleY, pixelResult.error().message())
                            );
                        }
                        
                        int kernelIdx = k + halfKernel;
                        sumR += r * m_kernel[kernelIdx];
                        sumG += g * m_kernel[kernelIdx];
                        sumB += b * m_kernel[kernelIdx];
                        if (hasAlpha) {
                            sumA += a * m_kernel[kernelIdx];
                        }
                    }
                    
                    auto setResult = colorResult.setPixel(x, y, 
                        static_cast<uint8_t>(sumR), 
                        static_cast<uint8_t>(sumG), 
                        static_cast<uint8_t>(sumB), 
                        hasAlpha ? static_cast<uint8_t>(sumA) : 255
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
            std::format("Gaussian blur failed: {}", e.what())
        );
    }
}

std::string_view GaussianBlurFilter::getName() const {
    return "GaussianBlur";
}

std::unique_ptr<FilterStrategy> GaussianBlurFilter::clone() const {
    return std::make_unique<GaussianBlurFilter>(m_sigma, m_kernelSize);
}

float GaussianBlurFilter::getSigma() const noexcept {
    return m_sigma;
}

int GaussianBlurFilter::getKernelSize() const noexcept {
    return m_kernelSize;
}

std::span<const float> GaussianBlurFilter::getKernel() const noexcept {
    return std::span<const float>(m_kernel);
}

void GaussianBlurFilter::generateKernel() {
    m_kernel.resize(m_kernelSize);

    float sum = 0.0f;
    int halfKernel = m_kernelSize / 2;

    // Generate 1D Gaussian kernel
    for (int i = 0; i < m_kernelSize; ++i) {
        int x = i - halfKernel;
        m_kernel[i] = std::exp(-(x * x) / (2.0f * m_sigma * m_sigma));
        sum += m_kernel[i];
    }

    // Normalize the kernel
    for (int i = 0; i < m_kernelSize; ++i) {
        m_kernel[i] /= sum;
    }
}

} // namespace DIPAL

