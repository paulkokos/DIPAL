// include/DIPAL/IO/JPEGImageIO.hpp
#ifndef DIPAL_JPEG_IMAGE_IO_HPP
#define DIPAL_JPEG_IMAGE_IO_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"

#include <memory>
#include <string_view>

namespace DIPAL {

/**
 * @brief Handler for JPEG image format
 */
class JPEGImageIO {
public:
    /**
     * @brief Load a JPEG image from a file
     * @param filename Path to the image file
     * @return Result containing the loaded image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<Image>> load(std::string_view filename);

    /**
     * @brief Save an image as JPEG
     * @param image The image to save
     * @param filename Path to the destination file
     * @param quality JPEG quality (0-100, default 90)
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult save(const Image& image,
                                         std::string_view filename,
                                         int quality = 90);
};

}  // namespace DIPAL

#endif  // DIPAL_JPEG_IMAGE_IO_HPP
