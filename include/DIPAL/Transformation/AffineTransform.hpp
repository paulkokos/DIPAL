// include/DIPAL/Transformation/AffineTransform.hpp
#ifndef DIPAL_AFFINE_TRANSFORM_HPP
#define DIPAL_AFFINE_TRANSFORM_HPP

#include "../Core/Error.hpp"
#include "../Image/Image.hpp"
#include "Transformations.hpp"

#include <array>
#include <functional>
#include <memory>
#include <string_view>

namespace DIPAL {

/**
 * @brief Affine transformation for images
 *
 * Represents an affine transformation with a 2x3 matrix:
 * [a b c]
 * [d e f]
 *
 * Where the transformation is:
 * x' = a*x + b*y + c
 * y' = d*x + e*y + f
 */
class AffineTransform : public ImageTransform {
public:
    /**
     * @brief Create an affine transformation with a transformation matrix
     * @param matrix Transformation matrix (2x3 array: [a,b,c,d,e,f])
     * @param method Interpolation method to use
     * @param outputWidth Width of output image (0 for auto-sizing)
     * @param outputHeight Height of output image (0 for auto-sizing)
     */
    AffineTransform(const std::array<float, 6>& matrix,
                    InterpolationMethod method = InterpolationMethod::Bilinear,
                    int outputWidth = 0,
                    int outputHeight = 0);

    /**
     * @brief Create an identity transformation (no change)
     * @return Identity transformation
     */
    static AffineTransform identity();

    /**
     * @brief Create a scaling transformation
     * @param scaleX X scale factor
     * @param scaleY Y scale factor
     * @param method Interpolation method
     * @return Scaling transformation
     */
    static AffineTransform scaling(float scaleX,
                                   float scaleY,
                                   InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a rotation transformation
     * @param angleDegrees Rotation angle in degrees (positive = counterclockwise)
     * @param centerX X coordinate of rotation center
     * @param centerY Y coordinate of rotation center
     * @param method Interpolation method
     * @return Rotation transformation
     */
    static AffineTransform rotation(float angleDegrees,
                                    float centerX = 0.0f,
                                    float centerY = 0.0f,
                                    InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a translation transformation
     * @param dx X translation amount
     * @param dy Y translation amount
     * @return Translation transformation
     */
    static AffineTransform translation(float dx, float dy);

    /**
     * @brief Create a shearing transformation
     * @param shearX X shear factor
     * @param shearY Y shear factor
     * @param method Interpolation method
     * @return Shearing transformation
     */
    static AffineTransform shearing(float shearX,
                                    float shearY,
                                    InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Apply the affine transformation
     * @param image Input image
     * @return Result containing the transformed image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the transformation name
     * @return "AffineTransform"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Get the transformation matrix
     * @return 2x3 matrix as array of 6 float values [a,b,c,d,e,f]
     */
    [[nodiscard]] const std::array<float, 6>& getMatrix() const noexcept;

    /**
     * @brief Get interpolation method
     * @return The interpolation method used
     */
    [[nodiscard]] InterpolationMethod getMethod() const noexcept;

    /**
     * @brief Get output width
     * @return Output width (0 means auto-calculated)
     */
    [[nodiscard]] int getOutputWidth() const noexcept;

    /**
     * @brief Get output height
     * @return Output height (0 means auto-calculated)
     */
    [[nodiscard]] int getOutputHeight() const noexcept;

    /**
     * @brief Compose this transformation with another
     * @param other The transformation to apply after this one
     * @return A new transformation representing the composition
     */
    [[nodiscard]] AffineTransform compose(const AffineTransform& other) const;

    /**
     * @brief Get the inverse transformation
     * @return Result containing the inverse transformation or error
     */
    [[nodiscard]] Result<AffineTransform> inverse() const;

private:
    std::array<float, 6> m_matrix;  ///< Transformation matrix [a,b,c,d,e,f]
    InterpolationMethod m_method;   ///< Interpolation method
    int m_outputWidth;              ///< Output width (0 = auto)
    int m_outputHeight;             ///< Output height (0 = auto)

    /**
     * @brief Calculate bounds of a transformed image
     * @param width Source image width
     * @param height Source image height
     * @return Pair of width and height for output image
     */
    [[nodiscard]] std::pair<int, int> calculateBounds(int width, int height) const;

    /**
     * @brief Create a mapping function for this affine transformation
     * @return Mapping function from destination to source coordinates
     */
    [[nodiscard]] std::function<std::pair<float, float>(float, float, int, int, int, int)>
    createMappingFunction() const;
};

}  // namespace DIPAL

#endif  // DIPAL_AFFINE_TRANSFORM_HPP
