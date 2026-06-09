// src/Filters/ClosingFilter.cpp
#include "../../include/DIPAL/Filters/ClosingFilter.hpp"
#include "../../include/DIPAL/Filters/ErosionFilter.hpp"
#include "../../include/DIPAL/Filters/DilationFilter.hpp"

namespace DIPAL {

ClosingFilter::ClosingFilter(StructuringElement se) : m_se(std::move(se)) {}

Result<std::unique_ptr<Image>> ClosingFilter::apply(const Image& image) const {
    DilationFilter dilate(m_se);
    ErosionFilter  erode(m_se);

    auto dilated = dilate.apply(image);
    if (!dilated) return dilated;

    return erode.apply(*dilated.value());
}

std::string_view ClosingFilter::getName() const { return "ClosingFilter"; }

std::unique_ptr<FilterStrategy> ClosingFilter::clone() const {
    return std::make_unique<ClosingFilter>(m_se);
}

} // namespace DIPAL
