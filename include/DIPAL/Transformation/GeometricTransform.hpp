// include/DIPAL/Transformation/GeometricTransform.hpp
#ifndef DIPAL_GEOMETRIC_TRANSFORM_HPP
#define DIPAL_GEOMETRIC_TRANSFORM_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"
#include "Transformations.hpp"

#include <functional>
#include <memory>
#include <string_view>

namespace DIPAL {

/**
 * @brief Geometric transformation using a custom mapping function
 *
 * This class allows for arbitrary geometric transformations defined
 * by a mapping function from destination coordinates to source coordinates.
 */
class GeometricTransform : public ImageTransform {
public:
    /**
     * @brief Create a geometric transformation
     * @param width Output image width
     * @param height Output image height
     * @param mappingFunc Function that maps output coordinates to input coordinates
     * @param method Interpolation method to use
     */
    GeometricTransform(int width,
                       int height,
                       std::function<std::pair<float, float>(float, float)> mappingFunc,
                       InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a polar to cartesian transformation
     * @param outputWidth Width of output image
     * @param outputHeight Height of output image
     * @param centerX X coordinate of the center in source image
     * @param centerY Y coordinate of the center in source image
     * @param method Interpolation method
     * @return Polar to cartesian transformation
     */
    static GeometricTransform polarToCartesian(
        int outputWidth,
        int outputHeight,
        float centerX,
        float centerY,
        InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a cartesian to polar transformation
     * @param outputWidth Width of output image
     * @param outputHeight Height of output image
     * @param centerX X coordinate of the center in source image
     * @param centerY Y coordinate of the center in source image
     * @param method Interpolation method
     * @return Cartesian to polar transformation
     */
    static GeometricTransform cartesianToPolar(
        int outputWidth,
        int outputHeight,
        float centerX,
        float centerY,
        InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a barrel distortion transformation
     * @param strength Distortion strength (positive for barrel, negative for pincushion)
     * @param method Interpolation method
     * @return Barrel distortion transformation
     */
    static GeometricTransform barrelDistortion(
        float strength,
        InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a perspective transformation
     * @param srcQuad Four source points defining a quadrilateral
     * @param dstQuad Four destination points defining a quadrilateral
     * @param outputWidth Width of output image
     * @param outputHeight Height of output image
     * @param method Interpolation method
     * @return Perspective transformation
     */
    static GeometricTransform perspective(
        const std::array<std::pair<float, float>, 4>& srcQuad,
        const std::array<std::pair<float, float>, 4>& dstQuad,
        int outputWidth,
        int outputHeight,
        InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a fish-eye distortion transformation
     * @param fovDegrees Field of view in degrees
     * @param outputWidth Width of output image
     * @param outputHeight Height of output image
     * @param method Interpolation method
     * @return Fish-eye transformation
     */
    static GeometricTransform fishEye(float fovDegrees,
                                      int outputWidth,
                                      int outputHeight,
                                      InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Apply the geometric transformation
     * @param image Input image
     * @return Result containing the transformed image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the transformation name
     * @return "GeometricTransform"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Get output width
     * @return Output image width
     */
    [[nodiscard]] int getWidth() const noexcept;

    /**
     * @brief Get output height
     * @return Output image height
     */
    [[nodiscard]] int getHeight() const noexcept;

    /**
     * @brief Get interpolation method
     * @return The interpolation method used
     */
    [[nodiscard]] InterpolationMethod getMethod() const noexcept;

private:
    int m_width;
    int m_height;
    std::function<std::pair<float, float>(float, float)> m_mappingFunc;
    InterpolationMethod m_method;

    /**
     * @brief Convert the map function from normalized to pixel coordinates
     * @param srcWidth Source image width
     * @param srcHeight Source image height
     * @return Function mapping pixel coordinates
     */
    [[nodiscard]] std::function<std::pair<float, float>(int, int)> createPixelMapping(
        int srcWidth,
        int srcHeight) const;
};

}  // namespace DIPAL

#endif  // DIPAL_GEOMETRIC_TRANSFORM_HPP
