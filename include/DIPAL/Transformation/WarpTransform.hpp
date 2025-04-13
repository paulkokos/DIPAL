// include/DIPAL/Transformation/WarpTransform.hpp
#ifndef DIPAL_WARP_TRANSFORM_HPP
#define DIPAL_WARP_TRANSFORM_HPP

#include "../Core/Error.hpp"
#include "../Core/Types.hpp"
#include "../Image/Image.hpp"
#include "Transformations.hpp"

#include <functional>
#include <memory>
#include <string_view>
#include <vector>

namespace DIPAL {

/**
 * @brief Type of warping algorithm to use
 */
enum class WarpMethod {
    ThinPlateSpline,  ///< Thin-plate spline interpolation
    MeshWarp,         ///< Mesh-based warping
    Triangulation     ///< Delaunay triangulation based warping
};

/**
 * @brief Image warping transformation using control points
 */
class WarpTransform : public ImageTransform {
public:
    /**
     * @brief Create a warp transformation using control points
     * @param sourcePoints Source control points
     * @param destPoints Destination control points (must have same count as sourcePoints)
     * @param method Warping method to use
     * @param interpolation Interpolation method to use
     */
    WarpTransform(const std::vector<Point>& sourcePoints,
                  const std::vector<Point>& destPoints,
                  WarpMethod method = WarpMethod::ThinPlateSpline,
                  InterpolationMethod interpolation = InterpolationMethod::Bilinear);

    /**
     * @brief Create a mesh-based warp transformation
     * @param meshWidth Horizontal mesh divisions
     * @param meshHeight Vertical mesh divisions
     * @param sourcePoints Source control points for mesh vertices
     * @param destPoints Destination control points for mesh vertices
     * @param interpolation Interpolation method to use
     * @return Mesh-based warp transformation
     */
    static WarpTransform createMeshWarp(
        int meshWidth,
        int meshHeight,
        const std::vector<Point>& sourcePoints,
        const std::vector<Point>& destPoints,
        InterpolationMethod interpolation = InterpolationMethod::Bilinear);

    /**
     * @brief Create a thin-plate spline warp transformation
     * @param sourcePoints Source control points
     * @param destPoints Destination control points
     * @param stiffness Stiffness parameter (higher values make less flexible warping)
     * @param interpolation Interpolation method to use
     * @return Thin-plate spline warp transformation
     */
    static WarpTransform createThinPlateSpline(
        const std::vector<Point>& sourcePoints,
        const std::vector<Point>& destPoints,
        float stiffness = 0.0f,
        InterpolationMethod interpolation = InterpolationMethod::Bilinear);

    /**
     * @brief Apply the warp transformation
     * @param image Input image
     * @return Result containing the warped image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the transformation name
     * @return "WarpTransform"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Get the source control points
     * @return Vector of source points
     */
    [[nodiscard]] const std::vector<Point>& getSourcePoints() const noexcept;

    /**
     * @brief Get the destination control points
     * @return Vector of destination points
     */
    [[nodiscard]] const std::vector<Point>& getDestPoints() const noexcept;

    /**
     * @brief Get the warping method
     * @return Warping method
     */
    [[nodiscard]] WarpMethod getWarpMethod() const noexcept;

    /**
     * @brief Get interpolation method
     * @return The interpolation method used
     */
    [[nodiscard]] InterpolationMethod getInterpolationMethod() const noexcept;

private:
    std::vector<Point> m_sourcePoints;
    std::vector<Point> m_destPoints;
    WarpMethod m_warpMethod;
    InterpolationMethod m_interpolationMethod;

    // Private methods for different warping algorithms

    /**
     * @brief Apply thin-plate spline warping
     * @param image Input image
     * @return Result containing the warped image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> applyThinPlateSpline(const Image& image) const;

    /**
     * @brief Apply mesh-based warping
     * @param image Input image
     * @return Result containing the warped image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> applyMeshWarp(const Image& image) const;

    /**
     * @brief Apply triangulation-based warping
     * @param image Input image
     * @return Result containing the warped image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> applyTriangulation(const Image& image) const;
};

}  // namespace DIPAL

#endif  // DIPAL_WARP_TRANSFORM_HPP
