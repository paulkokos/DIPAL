// src/Filters/GaussianBlurFilter.cpp
#include "../../include/DIPAL/Filters/GaussianBlurFilter.hpp"

#include <cmath>
#include <stdexcept>

namespace DIPAL {

GaussianBlurFilter::GaussianBlurFilter(float sigma, int kernelSize)
    : m_sigma(sigma), m_kernelSize(kernelSize) {
    // Kernel size must be odd
    if (kernelSize % 2 == 0) {
        throw std::invalid_argument("Kernel size must be odd");
    }

    generateKernel();
}

std::unique_ptr<Image> GaussianBlurFilter::apply(const Image& image) const {
    auto result = image.clone();

    // Create a temporary image for horizontal pass
    auto temp = image.clone();

    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();

    int halfKernel = m_kernelSize / 2;

    // For simplicity, this is a non-optimized implementation
    // A real implementation would handle different image types more efficiently

    // Horizontal pass
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                float sum = 0.0f;

                for (int k = -halfKernel; k <= halfKernel; ++k) {
                    int sampleX = std::max(0, std::min(width - 1, x + k));

                    // This is a simplified access - in a real implementation, you'd access
                    // pixel data more appropriately based on the image type
                    uint8_t* srcData = const_cast<uint8_t*>(image.getData());
                    // uint8_t* tempData = const_cast<uint8_t*>(temp->getData());

                    int srcIdx = (y * width + sampleX) * channels + c;
                    int kernelIdx = k + halfKernel;

                    sum += srcData[srcIdx] * m_kernel[kernelIdx];
                }

                int tempIdx = (y * width + x) * channels + c;
                uint8_t* tempData = const_cast<uint8_t*>(temp->getData());
                tempData[tempIdx] = static_cast<uint8_t>(sum);
            }
        }
    }

    // Vertical pass
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                float sum = 0.0f;

                for (int k = -halfKernel; k <= halfKernel; ++k) {
                    int sampleY = std::max(0, std::min(height - 1, y + k));

                    uint8_t* tempData = const_cast<uint8_t*>(temp->getData());
                    // uint8_t* resultData = const_cast<uint8_t*>(result->getData());

                    int tempIdx = (sampleY * width + x) * channels + c;
                    int kernelIdx = k + halfKernel;

                    sum += tempData[tempIdx] * m_kernel[kernelIdx];
                }

                int resultIdx = (y * width + x) * channels + c;
                uint8_t* resultData = const_cast<uint8_t*>(result->getData());
                resultData[resultIdx] = static_cast<uint8_t>(sum);
            }
        }
    }

    return result;
}

std::string GaussianBlurFilter::getName() const {
    return "GaussianBlur";
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

}  // namespace DIPAL
