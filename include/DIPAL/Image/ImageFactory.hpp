// include/DIPAL/Image/ImageFactory.hpp
#ifndef DIPAL_IMAGE_FACTORY_HPP
#define DIPAL_IMAGE_FACTORY_HPP

#include <memory>
#include <string>
#include <string_view>

#include "ColorImage.hpp"
#include "GrayscaleImage.hpp"
#include "Image.hpp"
#include "../Core/Error.hpp"

namespace DIPAL {

/**
 * @brief Factory class for creating and converting images
 */
class ImageFactory {
public:
    /**
     * @brief Create a new image of the specified type
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param type Image type
     * @return Result containing the created image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<Image>> create(int width, int height, Image::Type type);

    /**
     * @brief Create a new grayscale image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @return Result containing the created grayscale image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<GrayscaleImage>> createGrayscale(int width, int height);

    /**
     * @brief Create a new color image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param hasAlpha Whether to include an alpha channel
     * @return Result containing the created color image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<ColorImage>> createColor(int width, int height, bool hasAlpha = false);

    /**
     * @brief Convert a color image to grayscale
     * @param image The color image to convert
     * @return Result containing the grayscale image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<GrayscaleImage>> toGrayscale(const ColorImage& image);

    /**
     * @brief Convert a grayscale image to color
     * @param image The grayscale image to convert
     * @param hasAlpha Whether to include an alpha channel
     * @return Result containing the color image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<ColorImage>> toColor(const GrayscaleImage& image, bool hasAlpha = false);

    /**
     * @brief Load an image from a file
     * @param filename Path to the image file
     * @return Result containing the loaded image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<Image>> loadImage(std::string_view filename);

    /**
     * @brief Save an image to a file
     * @param image The image to save
     * @param filename Path to the destination file
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult saveImage(const Image& image, std::string_view filename);
};

} // namespace DIPAL

#endif // DIPAL_IMAGE_FACTORY_HPP
