// src/Filters/UnsharpMaskFilter.cpp

#include "../../include/DIPAL/Filters/UnsharpMaskFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {
  UnsharpMaskFilter::UnsharpMaskFilter(float amount, float radius, uint8_t threshold) {
    if (amount  < 0.0f) {
      throw std::invalid_argument();
    }

  }
  
}
