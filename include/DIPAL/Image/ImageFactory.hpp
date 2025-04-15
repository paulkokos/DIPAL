// include/DIPAL/Image/ImageFactory.hpp
#ifndef DIPAL_IMAGE_FACTORY_HPP
#define DIPAL_IMAGE_FACTORY_HPP

#include "../Core/Error.hpp"
#include "Image.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace DIPAL {

// Forward declarations
class BinaryImage;
class GrayscaleImage;
class ColorImage;

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
    [[nodiscard]] static Result<std::unique_ptr<Image>> create(int width,
                                                               int height,
                                                               Image::Type type);

    /**
     * @brief Create a new binary image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @return Result containing the created binary image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<BinaryImage>> createBinary(int width, int height);

    /**
     * @brief Create a new grayscale image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @return Result containing the created grayscale image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<GrayscaleImage>> createGrayscale(int width,
                                                                                 int height);

    /**
     * @brief Create a new color image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param hasAlpha Whether to include an alpha channel
     * @return Result containing the created color image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<ColorImage>> createColor(int width,
                                                                         int height,
                                                                         bool hasAlpha = false);

    /**
     * @brief Convert a color image to grayscale
     * @param image The color image to convert
     * @return Result containing the grayscale image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<GrayscaleImage>> toGrayscale(
        const ColorImage& image);

    /**
     * @brief Convert a grayscale image to color
     * @param image The grayscale image to convert
     * @param hasAlpha Whether to include an alpha channel
     * @return Result containing the color image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<ColorImage>> toColor(const GrayscaleImage& image,
                                                                     bool hasAlpha = false);

    /**
     * @brief Convert a grayscale image to binary using thresholding
     * @param image The grayscale image to convert
     * @param threshold The threshold value (0-255)
     * @param invert Whether to invert the result (default: false)
     * @return Result containing the binary image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<BinaryImage>> toBinary(const GrayscaleImage& image,
                                                                       uint8_t threshold = 128,
                                                                       bool invert = false);

    /**
     * @brief Convert a binary image to grayscale
     * @param image The binary image to convert
     * @param whiteValue The gray value to use for white pixels (default: 255)
     * @param blackValue The gray value to use for black pixels (default: 0)
     * @return Result containing the grayscale image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<GrayscaleImage>>
    fromBinary(const BinaryImage& image, uint8_t whiteValue = 255, uint8_t blackValue = 0);

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

    /**
     * @brief Save an image to a file with quality setting
     * @param image The image to save
     * @param filename Path to the destination file
     * @param quality Quality setting (0-100, only applies to formats that support it like JPEG)
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult saveImage(const Image& image,
                                              std::string_view filename,
                                              int quality);
};

}  // namespace DIPAL

#endif  // DIPAL_IMAGE_FACTORY_HPP
