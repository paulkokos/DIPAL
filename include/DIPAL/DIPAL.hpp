// include/DIPAL/DIPAL.hpp
#ifndef DIPAL_HPP
#define DIPAL_HPP

// Core includes
#include "Core/Concepts.hpp"
#include "Core/Core.hpp"
#include "Core/Error.hpp"
#include "Core/Result.hpp"
#include "Core/Types.hpp"

// Image includes - note the order to handle dependencies
#include "Image/BinaryImage.hpp"
#include "Image/ColorImage.hpp"
#include "Image/GrayscaleImage.hpp"
#include "Image/Image.hpp"
#include "Image/ImageFactory.hpp"
#include "Image/ImageView.hpp"
#include "Image/PixelIterator.hpp"

// Filter includes
#include "Filters/FilterStrategy.hpp"
#include "Filters/GaussianBlurFilter.hpp"
#include "Filters/MedianFilter.hpp"
#include "Filters/SobelFilter.hpp"
#include "Filters/UnsharpMaskFilter.hpp"

// I/O Includes
#include "IO/BMPImageIO.hpp"
#include "IO/ImageIO.hpp"
#include "IO/PPMImageIO.hpp"

// Processing includes
#include "ImageProcessor/FilterCommand.hpp"
#include "ImageProcessor/ImageProcessor.hpp"
#include "ImageProcessor/ParallelProcessor.hpp"
#include "ImageProcessor/ProcessingCommand.hpp"

// Observer includes
#include "Observer/ProcessingObserver.hpp"
#include "Observer/ProgressObserver.hpp"

// Transformation includes
#include "Transformation/AffineTransform.hpp"
#include "Transformation/GeometricTransform.hpp"
#include "Transformation/Interpolation.hpp"
#include "Transformation/ResizeTransform.hpp"
#include "Transformation/RotateTransform.hpp"
#include "Transformation/Transformations.hpp"  // Must come first for common definitions
#include "Transformation/WarpTransform.hpp"

// Utility includes
#include "Utils/Concurrency.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MemoryUtils.hpp"
#include "Utils/Profiler.hpp"
#include "Utils/Utils.hpp"

// Color includes
#include "Color/ColorConversions.hpp"
#include "Color/ColorSpace.hpp"
#include "Color/ColorTransform.hpp"

// Version information
#define DIPAL_VERSION_MAJOR 0
#define DIPAL_VERSION_MINOR 1
#define DIPAL_VERSION_PATCH 0

namespace DIPAL {
/**
 * @brief Get the library version string
 * @return Version string in format "MAJOR.MINOR.PATCH"
 */
inline const char* getVersion() {
    return "0.1.0";
}
}  // namespace DIPAL

#endif  // DIPAL_HPP
