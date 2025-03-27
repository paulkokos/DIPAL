// src/IO/ImageIO.cpp
#include "../../include/DIPAL/IO/ImageIO.hpp"
#include "../../include/DIPAL/IO/PPMImageIO.hpp"
#include "../../include/DIPAL/IO/BMPImageIO.hpp"

#include <filesystem>
#include <algorithm>
#include <cctype>
#include <format>

namespace DIPAL {

Result<std::unique_ptr<Image>> ImageIO::load(std::string_view filename) {
    // Check if file exists
    std::filesystem::path path(filename);
    if (!std::filesystem::exists(path)) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::FileNotFound,
            std::format("File not found: {}", filename)
        );
    }
    
    // Get the file format from the extension
    auto format = getFormatFromExtension(filename);
    
    if (format == "ppm" || format == "pgm" || format == "pbm") {
        // Use PPM loader
        return PPMImageIO::load(filename);
    } else if (format == "bmp") {
        // Use BMP loader
        return BMPImageIO::load(filename);
    } else {
        // Unsupported format
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("Unsupported image format: {}", format)
        );
    }
}

VoidResult ImageIO::save(const Image& image, std::string_view filename) {
    // Get the file format from the extension
    auto format = getFormatFromExtension(filename);
    
    if (format == "ppm" || format == "pgm" || format == "pbm") {
        // Use PPM saver
        return PPMImageIO::save(image, filename);
    } else if (format == "bmp") {
        // Use BMP saver
        return BMPImageIO::save(image, filename);
    } else {
        // Unsupported format
        return makeVoidErrorResult(
            ErrorCode::UnsupportedFormat,
            std::format("Unsupported image format: {}", format)
        );
    }
}

std::string_view ImageIO::getFormatFromExtension(std::string_view filename) {
    // Get the extension from the filename
    auto path = std::filesystem::path(filename);
    auto extension = path.extension().string();
    
    // Convert to lowercase
    std::transform(extension.begin(), extension.end(), extension.begin(),
                 [](unsigned char c) { return std::tolower(c); });
    
    // Remove the leading dot
    if (!extension.empty() && extension[0] == '.') {
        extension = extension.substr(1);
    }
    
    return extension;
}

} // namespace DIPAL
