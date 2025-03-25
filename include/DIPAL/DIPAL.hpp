// include/DIPAL/DIPAL.hpp
#ifndef DIPAL_HPP
#define DIPAL_HPP

// Core includes
#include "Core/Core.hpp"

// Image includes
#include "Image/ColorImage.hpp"
#include "Image/GrayscaleImage.hpp"
#include "Image/Image.hpp"
#include "Image/ImageFactory.hpp"


// Filter includes

#include "Filters/FilterStrategy.hpp"
#include "Filters/GaussianBlurFilter.hpp"

// I/O Includes
#include "IO/ImageIO.hpp"
#include "IO/PPMImageIO.hpp"
#include "IO/BMPImageIO.hpp"

// Processing includes
#include "ImageProcessor/ProcessingCommand.hpp"

// Version information
#define DIPAL_VERSION_MAJOR 0
#define DIPAL_VERSION_MINOR 1
#define DIPAL_VERSION_PATCH 0

namespace DIPAL {
    inline const char* getVersion(){
        return "0.1.0";
    }
// Main library class
class DIPAL {
public:
    DIPAL();
    ~DIPAL();

    // Add main library functionality here

private:
    // Implementation details
};
}  // namespace DIPAL

#endif  // DIPAL_HPP
