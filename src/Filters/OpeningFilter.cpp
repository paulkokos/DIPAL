// src/Filters/OpeningFilter.cpp
#include "../../include/DIPAL/Filters/OpeningFilter.hpp"
#include "../../include/DIPAL/Filters/ErosionFilter.hpp"
#include "../../include/DIPAL/Filters/DilationFilter.hpp"

#include <format>

namespace DIPAL {

OpeningFilter::OpeningFilter(StructuringElement se) : m_se(std::move(se)) {}

Result<std::unique_ptr<Image>> OpeningFilter::apply(const Image& image) const {
    ErosionFilter  erode(m_se);
    DilationFilter dilate(m_se);

    auto eroded = erode.apply(image);
    if (!eroded) return eroded;

    return dilate.apply(*eroded.value());
}

std::string_view OpeningFilter::getName() const { return "OpeningFilter"; }

std::unique_ptr<FilterStrategy> OpeningFilter::clone() const {
    return std::make_unique<OpeningFilter>(m_se);
}

} // namespace DIPAL
