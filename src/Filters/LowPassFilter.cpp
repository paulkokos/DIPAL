// src/Filters/LowPassFilter.cpp
#include "../../include/DIPAL/Filters/LowPassFilter.hpp"
#include "../../include/DIPAL/Filters/GaussianBlurFilter.hpp"

#include <cmath>
#include <format>

namespace DIPAL {

LowPassFilter::LowPassFilter(float sigma, int kernelSize)
    : m_sigma(sigma), m_kernelSize(kernelSize)
{
    if (sigma <= 0.0f)
        throw std::invalid_argument(
            std::format("LowPassFilter: sigma must be positive, got {}", sigma));
}

Result<std::unique_ptr<Image>> LowPassFilter::apply(const Image& image) const {
    if (image.getWidth() == 0 || image.getHeight() == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply low-pass filter to empty image");

    int kSize = m_kernelSize;
    if (kSize <= 0) kSize = static_cast<int>(std::ceil(6.0f * m_sigma)) | 1;
    if (kSize % 2 == 0) ++kSize;

    GaussianBlurFilter blur(m_sigma, kSize);
    return blur.apply(image);
}

std::string_view LowPassFilter::getName() const { return "LowPassFilter"; }

std::unique_ptr<FilterStrategy> LowPassFilter::clone() const {
    return std::make_unique<LowPassFilter>(m_sigma, m_kernelSize);
}

} // namespace DIPAL
