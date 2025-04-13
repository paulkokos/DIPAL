// include/DIPAL/Transformation/RotateTransform.hpp
#ifndef DIPAL_ROTATE_TRANSFORM_HPP
#define DIPAL_ROTATE_TRANSFORM_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"
#include "Transformations.hpp"

#include <cmath>
#include <memory>
#include <string_view>

namespace DIPAL {

/**
 * @brief Rotation center type
 */
enum class RotationCenter {
    Center,   ///< Rotate around image center
    TopLeft,  ///< Rotate around top-left corner
    Custom    ///< Rotate around custom point
};

/**
 * @brief Image rotation transformation
 */
class RotateTransform : public ImageTransform {
public:
    /**
     * @brief Create a rotation transformation
     * @param angle Rotation angle in degrees (positive = counterclockwise)
     * @param center Rotation center type
     * @param method Interpolation method
     * @param resizeOutput Whether to resize the output to fit rotated content
     */
    RotateTransform(float angle,
                    RotationCenter center = RotationCenter::Center,
                    InterpolationMethod method = InterpolationMethod::Bilinear,
                    bool resizeOutput = true);

    /**
     * @brief Create a rotation transformation with custom center point
     * @param angle Rotation angle in degrees (positive = counterclockwise)
     * @param centerX X-coordinate of rotation center
     * @param centerY Y-coordinate of rotation center
     * @param method Interpolation method
     * @param resizeOutput Whether to resize the output to fit rotated content
     */
    RotateTransform(float angle,
                    float centerX,
                    float centerY,
                    InterpolationMethod method = InterpolationMethod::Bilinear,
                    bool resizeOutput = true);

    /**
     * @brief Apply the rotation transformation
     * @param image Input image
     * @return Result containing the rotated image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the transformation name
     * @return "RotateTransform"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Get rotation angle
     * @return Angle in degrees
     */
    [[nodiscard]] float getAngle() const noexcept;

    /**
     * @brief Get rotation center type
     * @return Rotation center type
     */
    [[nodiscard]] RotationCenter getCenter() const noexcept;

    /**
     * @brief Get custom center X coordinate
     * @return X coordinate of custom center (if applicable)
     */
    [[nodiscard]] float getCenterX() const noexcept;

    /**
     * @brief Get custom center Y coordinate
     * @return Y coordinate of custom center (if applicable)
     */
    [[nodiscard]] float getCenterY() const noexcept;

    /**
     * @brief Get interpolation method
     * @return The interpolation method used
     */
    [[nodiscard]] InterpolationMethod getMethod() const noexcept;

    /**
     * @brief Check if output is being resized
     * @return True if output is resized to fit rotated content
     */
    [[nodiscard]] bool isResizeOutput() const noexcept;

private:
    float m_angle;                 ///< Rotation angle in degrees
    RotationCenter m_centerType;   ///< Type of rotation center
    float m_centerX;               ///< X coordinate of rotation center
    float m_centerY;               ///< Y coordinate of rotation center
    InterpolationMethod m_method;  ///< Interpolation method
    bool m_resizeOutput;           ///< Whether to resize output

    /**
     * @brief Calculate the output dimensions for a rotated image
     * @param width Original image width
     * @param height Original image height
     * @param angle Rotation angle in radians
     * @return Pair of width and height for the output image
     */
    static std::pair<int, int> calculateRotatedDimensions(int width,
                                                          int height,
                                                          float angleRadians);

    /**
     * @brief Create a rotation mapping function
     * @param srcWidth Source image width
     * @param srcHeight Source image height
     * @param dstWidth Destination image width
     * @param dstHeight Destination image height
     * @param angle Rotation angle in radians
     * @param centerX X coordinate of rotation center in source image
     * @param centerY Y coordinate of rotation center in source image
     * @return Mapping function from destination to source coordinates
     */
    static std::function<std::pair<float, float>(float, float, int, int, int, int)>
    createRotationMapping(float angle, float centerX, float centerY);
};

}  // namespace DIPAL

#endif  // DIPAL_ROTATE_TRANSFORM_HPP
