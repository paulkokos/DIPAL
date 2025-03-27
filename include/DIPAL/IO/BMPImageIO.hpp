// include/DIPAL/IO/BMPImageIO.hpp
#ifndef DIPAL_BMP_IMAGE_IO_HPP
#define DIPAL_BMP_IMAGE_IO_HPP

#include <string_view>
#include <memory>

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"

namespace DIPAL {

/**
 * @brief Handler for BMP image format
 */
class BMPImageIO {
public:
    /**
     * @brief Load a BMP image from a file
     * @param filename Path to the image file
     * @return Result containing the loaded image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<Image>> load(std::string_view filename);

    /**
     * @brief Save an image as BMP
     * @param image The image to save
     * @param filename Path to the destination file
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult save(const Image& image, std::string_view filename);
};

} // namespace DIPAL

#endif // DIPAL_BMP_IMAGE_IO_HPP
