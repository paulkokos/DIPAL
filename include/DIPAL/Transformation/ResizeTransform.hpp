// include/DIPAL/Transformation/ResizeTransform.hpp
#ifndef DIPAL_RESIZE_TRANSFORM_HPP
#define DIPAL_RESIZE_TRANSFORM_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"
#include "Transformations.hpp"  // Include for InterpolationMethod and ImageTransform

#include <memory>
#include <string_view>

namespace DIPAL {

/**
 * @brief Image resizing transformation
 */
class ResizeTransform : public ImageTransform {
public:
    /**
     * @brief Create a resize transformation
     * @param newWidth New width in pixels
     * @param newHeight New height in pixels
     * @param method Interpolation method
     */
    ResizeTransform(int newWidth,
                    int newHeight,
                    InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Apply the resize transformation
     * @param image Input image
     * @return Result containing the resized image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the transformation name
     * @return "ResizeTransform"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Get target width
     * @return Target width in pixels
     */
    [[nodiscard]] int getWidth() const noexcept;

    /**
     * @brief Get target height
     * @return Target height in pixels
     */
    [[nodiscard]] int getHeight() const noexcept;

    /**
     * @brief Get interpolation method
     * @return The interpolation method used
     */
    [[nodiscard]] InterpolationMethod getMethod() const noexcept;

private:
    int m_newWidth;
    int m_newHeight;
    InterpolationMethod m_method;

    // Helper methods for different interpolation methods
    [[nodiscard]] Result<std::unique_ptr<Image>> resizeNearestNeighbor(const Image& image) const;
    [[nodiscard]] Result<std::unique_ptr<Image>> resizeBilinear(const Image& image) const;
    [[nodiscard]] Result<std::unique_ptr<Image>> resizeBicubic(const Image& image) const;
};

}  // namespace DIPAL

#endif  // DIPAL_RESIZE_TRANSFORM_HPP
