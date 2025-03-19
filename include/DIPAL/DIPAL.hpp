#ifndef DIPAL_HPP
#define DIPAL_HPP

// Main include file for DIPAL library
// Include all public API headers here

#include "Core/Core.hpp"
#include "Image/Image.hpp"
#include "Filters/Filters.hpp"
#include "Transformation/Transformations.hpp"
#include "Color/ColorConversions.hpp"
#include "IO/ImageIO.hpp"
#include "Utils/Utils.hpp"

// Version information
#define DIPAL_VERSION_MAJOR 0
#define DIPAL_VERSION_MINOR 1
#define DIPAL_VERSION_PATCH 0

namespace DIPAL {
    // Core library class
    class DIPAL {
    public:
        DIPAL();
        ~DIPAL();
        
        // Add main library functionality here
        
    private:
        // Implementation details
    };
}

#endif // DIPAL_HPP
