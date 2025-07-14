// include/DIPAL/IO/ImageIO.hpp
#ifndef DIPAL_IMAGE_IO_HPP
#define DIPAL_IMAGE_IO_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace DIPAL {

/**
 * @brief Static class for loading and saving images in various formats
 */
class ImageIO {
public:
    /**
     * @brief Load an image from a file
     * @param filename Path to the image file
     * @return Result containing the loaded image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<Image>> load(std::string_view filename);

    /**
     * @brief Save an image to a file
     * @param image The image to save
     * @param filename Path to the destination file
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult save(const Image& image, std::string_view filename);

    /**
     * @brief Save an image to a file with quality setting
     * @param image The image to save
     * @param filename Path to the destination file
     * @param quality Quality setting (0-100, only applies to formats that support it like JPEG)
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult save(const Image& image,
                                         std::string_view filename,
                                         [[maybe_unused]] int quality);

private:
    // Helper method to determine file format from extension
    [[nodiscard]] static std::string getFormatFromExtension(std::string_view filename);
};

}  // namespace DIPAL

#endif  // DIPAL_IMAGE_IO_HPP
