// include/DIPAL/IO/PPMImageIO.hpp
#ifndef DIPAL_PPM_IMAGE_IO_HPP
#define DIPAL_PPM_IMAGE_IO_HPP

#include <string_view>
#include <memory>

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"

namespace DIPAL {

/**
 * @brief Handler for PPM/PGM/PBM image formats
 */
class PPMImageIO {
public:
    /**
     * @brief Load a PPM/PGM/PBM image from a file
     * @param filename Path to the image file
     * @return Result containing the loaded image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<Image>> load(std::string_view filename);

    /**
     * @brief Save an image as PPM/PGM/PBM
     * @param image The image to save
     * @param filename Path to the destination file
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] static VoidResult save(const Image& image, std::string_view filename);
};

} // namespace DIPAL

#endif // DIPAL_PPM_IMAGE_IO_HPP
