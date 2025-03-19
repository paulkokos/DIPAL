// include/DIPAL/Image/ImageFactory.hpp
#ifndef DIPAL_IMAGE_FACTORY_HPP
#define DIPAL_IMAGE_FACTORY_HPP

#include <memory>
#include <string>

#include "ColorImage.hpp"
#include "GrayscaleImage.hpp"
#include "Image.hpp"

namespace DIPAL {

/**
 * @brief Factory class for creating and converting images
 */
class ImageFactory {
public:
    /*
     * @brief Create a new image of the specified type
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param type Image type
     * @return Unique pointer t the created image
     */
    static std::unique_ptr<Image> create(int width, int height, Image::Type type);

    /**
     * @brief Create a new grayscale image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @return Unique pointer to the created grayscale image
     */
    static std::unique_ptr<GrayscaleImage> createGrayscale(int width, int height);

    /*
     * @brief Create a new color image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param hasAlpha Whether to include an alpha channel
     * @return Unique pointer to the created color image
     */
    static std::unique_ptr<ColorImage> createColor(int width, int height, bool hasAlpha = false);

    /**
     * @brief Convert a color image to grayscale
     * @param image The color image to convert
     * @return A new grayscale image
     */
    static std::unique_ptr<GrayscaleImage> toGrayscale(const ColorImage& image);

    /**
     * @brief Convert a grayscale image to color
     * @param image The grayscale image to convert
     * @param hasAlpha Whether to include an alpha channel
     * @return A new color image
     */
    static std::unique_ptr<ColorImage> toColor(const GrayscaleImage& image, bool hasAlpha = false);
};

}  // namespace DIPAL

#endif  // DIPAL_IMAGE_FACTORY_HPP
