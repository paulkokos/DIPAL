// include/DIPAL/Transformation/Transformations.hpp
#ifndef DIPAL_TRANSFORMATIONS_HPP
#define DIPAL_TRANSFORMATIONS_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"

#include <memory>
#include <string_view>

namespace DIPAL {

/**
 * @brief Interpolation method for image transformations
 */
enum class InterpolationMethod {
    NearestNeighbor,  ///< Nearest neighbor interpolation (fastest, lowest quality)
    Bilinear,         ///< Bilinear interpolation (good balance)
    Bicubic           ///< Bicubic interpolation (slower, highest quality)
};

/**
 * @brief Base class for all image transformations
 */
class ImageTransform {
public:
    virtual ~ImageTransform() = default;

    /**
     * @brief Apply the transformation to an image
     * @param image The image to transform
     * @return Result containing the transformed image or error
     */
    virtual Result<std::unique_ptr<Image>> apply(const Image& image) const = 0;

    /**
     * @brief Get the name of the transformation
     * @return Transformation name
     */
    virtual std::string_view getName() const = 0;
};

}  // namespace DIPAL

#endif  // DIPAL_TRANSFORMATIONS_HPP
