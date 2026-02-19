// include/DIPAL/IO/PNGImageIO.hpp
#ifndef DIPAL_PNG_IMAGE_IO_HPP
#define DIPAL_PNG_IMAGE_IO_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"

#include <memory>
#include <string_view>

namespace DIPAL {

/**
 * @brief Handler for PNG image format
 */
class PNGImageIO {
public:
    /**
     * @brief Load a PNG image from a file
     * @param filename Path to the image file
     * @return Result containing the loaded image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<Image>> load(std::string_view filename);

    /**
     * @brief Save an image as PNG
     * @param image The image to save
     * @param filename Path to the destination file
     * @param compressionLevel PNG compression level (0-9, default 6)
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult save(const Image& image,
                                         std::string_view filename,
                                         int compressionLevel = 6);
};

}  // namespace DIPAL

#endif  // DIPAL_PNG_IMAGE_IO_HPP
