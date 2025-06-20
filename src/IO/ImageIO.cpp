// src/IO/ImageIO.cpp
#include "../../include/DIPAL/IO/ImageIO.hpp"

#include "../../include/DIPAL/IO/BMPImageIO.hpp"
#include "../../include/DIPAL/IO/PPMImageIO.hpp"

#ifdef DIPAL_HAS_JPEG
#include "../../include/DIPAL/IO/JPEGImageIO.hpp"
#endif

#ifdef DIPAL_HAS_PNG
#include "../../include/DIPAL/IO/PNGImageIO.hpp"
#endif

#ifdef DIPAL_HAS_TIFF
#include "../../include/DIPAL/IO/TIFFImageIO.hpp"
#endif

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <format>

namespace DIPAL {

Result<std::unique_ptr<Image>> ImageIO::load(std::string_view filename) {
    // Check if file exists
    std::filesystem::path path(filename);
    if (!std::filesystem::exists(path)) {
        return makeErrorResult<std::unique_ptr<Image>>(ErrorCode::FileNotFound,
                                                       std::format("File not found: {}", filename));
    }

    // Get the file format from the extension
    auto format = getFormatFromExtension(filename);

    if (format == "ppm" || format == "pgm" || format == "pbm") {
        // Use PPM loader
        return PPMImageIO::load(filename);
    } else if (format == "bmp") {
        // Use BMP loader
        return BMPImageIO::load(filename);
    }
#ifdef DIPAL_HAS_JPEG
    else if (format == "jpg" || format == "jpeg") {
        // Use JPEG loader
        return JPEGImageIO::load(filename);
    }
#endif
#ifdef DIPAL_HAS_PNG
    else if (format == "png") {
        // Use PNG loader
        return PNGImageIO::load(filename);
    }
#endif
#ifdef DIPAL_HAS_TIFF
    else if (format == "tif" || format == "tiff") {
        // Use TIFF loader
        return TIFFImageIO::load(filename);
    }
#endif
    else {
        // Unsupported format
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat, std::format("Unsupported image format: {}", format));
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
    }
#ifdef DIPAL_HAS_JPEG
    else if (format == "jpg" || format == "jpeg") {
        // Use JPEG saver with default quality 90
        return JPEGImageIO::save(image, filename, 90);
    }
#endif
#ifdef DIPAL_HAS_PNG
    else if (format == "png") {
        // Use PNG saver
        return PNGImageIO::save(image, filename);
    }
#endif
#ifdef DIPAL_HAS_TIFF
    else if (format == "tif" || format == "tiff") {
        // Use TIFF saver
        return TIFFImageIO::save(image, filename);
    }
#endif
    else {
        // Unsupported format
        return makeVoidErrorResult(ErrorCode::UnsupportedFormat,
                                   std::format("Unsupported image format: {}", format));
    }
}

// Overload save function to allow quality parameter for supported formats
VoidResult ImageIO::save(const Image& image, std::string_view filename, int quality) {
    // Get the file format from the extension
    auto format = getFormatFromExtension(filename);

#ifdef DIPAL_HAS_JPEG
    if (format == "jpg" || format == "jpeg") {
        // Use JPEG saver with specified quality
        return JPEGImageIO::save(image, filename, quality);
    }
#endif
    // For other formats, ignore quality parameter and use standard save
    return save(image, filename);
}

std::string_view ImageIO::getFormatFromExtension(std::string_view filename) {
    // Get the extension from the filename
    auto path = std::filesystem::path(filename);
    auto extension = path.extension().string();

    // Convert to lowercase
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // Remove the leading dot
    if (!extension.empty() && extension[0] == '.') {
        extension = extension.substr(1);
    }

    return extension;
}

}  // namespace DIPAL
