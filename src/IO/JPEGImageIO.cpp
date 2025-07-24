// src/IO/JPEGImageIO.cpp
#include "../../include/DIPAL/IO/JPEGImageIO.hpp"

#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <csetjmp>
#include <cstring>
#include <format>
#include <fstream>
#include <vector>

// Ensure JPEG support is enabled
#ifdef DIPAL_HAS_JPEG

// Include libjpeg header
extern "C" {
#include <jpeglib.h>
}

namespace DIPAL {

// Custom error handler for libjpeg
struct JPEGErrorManager {
    jpeg_error_mgr pub;     // "public" fields for libjpeg
    jmp_buf setjmp_buffer;  // for return to caller
    std::string errorMsg;   // error message to return to caller
};

// Custom error handler function for libjpeg
static void jpegErrorExit(j_common_ptr cinfo) {
    // cinfo->err really points to a JPEGErrorManager struct
    JPEGErrorManager* myerr = reinterpret_cast<JPEGErrorManager*>(cinfo->err);

    // Create error message
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message)(cinfo, buffer);
    myerr->errorMsg = buffer;

    // Return control to the setjmp point
    longjmp(myerr->setjmp_buffer, 1);
}

Result<std::unique_ptr<Image>> JPEGImageIO::load(std::string_view filename) {
    // Open the file
    FILE* infile = fopen(std::string(filename).c_str(), "rb");
    if (!infile) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::FileNotFound, std::format("Failed to open JPEG file: {}", filename));
    }

    try {
        // Setup decompression structures
        jpeg_decompress_struct cinfo;
        JPEGErrorManager jerr;

        // Setup error handling
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = jpegErrorExit;

        // Establish the setjmp return context for jpegErrorExit to use
        if (setjmp(jerr.setjmp_buffer)) {
            // If we get here, the JPEG code has signaled an error
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::InvalidFormat,
                std::format("JPEG decompression error: {}", jerr.errorMsg));
        }

        // Initialize JPEG decompression object
        jpeg_create_decompress(&cinfo);

        // Specify data source
        jpeg_stdio_src(&cinfo, infile);

        // Read file header
        jpeg_read_header(&cinfo, TRUE);

        // Start decompression
        jpeg_start_decompress(&cinfo);

        // Get image dimensions and color space
        int width = cinfo.output_width;
        int height = cinfo.output_height;
        int channels = cinfo.output_components;

        // Create the appropriate image type
        std::unique_ptr<Image> image;

        if (channels == 1) {
            // Grayscale image
            auto result = ImageFactory::createGrayscale(width, height);
            if (!result) {
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                fclose(infile);
                return result;
            }
            image = std::move(result.value());
        } else if (channels == 3) {
            // RGB image
            auto result = ImageFactory::createColor(width, height, false);
            if (!result) {
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                fclose(infile);
                return result;
            }
            image = std::move(result.value());
        } else {
            // Unsupported number of channels
            jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported JPEG color mode with {} channels", channels));
        }

        // Allocate buffer for one row
        std::vector<JSAMPLE> buffer(width * channels);
        JSAMPROW row_pointer[1] = {buffer.data()};

        // Process each scanline
        while (cinfo.output_scanline < cinfo.output_height) {
            int y = cinfo.output_scanline;

            // Read scanline
            jpeg_read_scanlines(&cinfo, row_pointer, 1);

            // Copy scanline to our image
            if (channels == 1) {
                // Grayscale
                auto& grayImage = static_cast<GrayscaleImage&>(*image);
                for (int x = 0; x < width; ++x) {
                    uint8_t value = buffer[x];
                    auto setResult = grayImage.setPixel(x, y, value);
                    if (!setResult) {
                        jpeg_finish_decompress(&cinfo);
                        jpeg_destroy_decompress(&cinfo);
                        fclose(infile);
                        return makeErrorResult<std::unique_ptr<Image>>(
                            setResult.error().code(),
                            std::format("Failed to set pixel at ({}, {}): {}",
                                        x,
                                        y,
                                        setResult.error().message()));
                    }
                }
            } else {
                // RGB
                auto& colorImage = static_cast<ColorImage&>(*image);
                for (int x = 0; x < width; ++x) {
                    uint8_t r = buffer[x * channels];
                    uint8_t g = buffer[x * channels + 1];
                    uint8_t b = buffer[x * channels + 2];

                    auto setResult = colorImage.setPixel(x, y, r, g, b);
                    if (!setResult) {
                        jpeg_finish_decompress(&cinfo);
                        jpeg_destroy_decompress(&cinfo);
                        fclose(infile);
                        return makeErrorResult<std::unique_ptr<Image>>(
                            setResult.error().code(),
                            std::format("Failed to set pixel at ({}, {}): {}",
                                        x,
                                        y,
                                        setResult.error().message()));
                    }
                }
            }
        }

        // Clean up
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

        return makeSuccessResult(std::move(image));
    } catch (const std::exception& e) {
        // Close file if still open
        if (infile) {
            fclose(infile);
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InternalError, std::format("Error reading JPEG file: {}", e.what()));
    }
}

VoidResult JPEGImageIO::save(const Image& image, std::string_view filename, int quality) {
    // Clamp quality value
    quality = std::clamp(quality, 0, 100);

    // Open the file
    FILE* outfile = fopen(std::string(filename).c_str(), "wb");
    if (!outfile) {
        return makeVoidErrorResult(ErrorCode::FileAccessDenied,
                                   std::format("Failed to create file: {}", filename));
    }

    try {
        // Setup compression structures
        jpeg_compress_struct cinfo;
        JPEGErrorManager jerr;

        // Setup error handling
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = jpegErrorExit;

        // Establish the setjmp return context for jpegErrorExit to use
        if (setjmp(jerr.setjmp_buffer)) {
            // If we get here, the JPEG code has signaled an error
            jpeg_destroy_compress(&cinfo);
            fclose(outfile);
            return makeVoidErrorResult(ErrorCode::ProcessingFailed,
                                       std::format("JPEG compression error: {}", jerr.errorMsg));
        }

        // Initialize JPEG compression object
        jpeg_create_compress(&cinfo);

        // Specify data destination
        jpeg_stdio_dest(&cinfo, outfile);

        // Set image parameters
        cinfo.image_width = image.getWidth();
        cinfo.image_height = image.getHeight();

        // Set color space based on image type
        if (image.getType() == Image::Type::Grayscale) {
            cinfo.input_components = 1;
            cinfo.in_color_space = JCS_GRAYSCALE;
        } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            cinfo.input_components = 3;  // RGB (we'll ignore alpha)
            cinfo.in_color_space = JCS_RGB;
        } else {
            jpeg_destroy_compress(&cinfo);
            fclose(outfile);
            return makeVoidErrorResult(ErrorCode::UnsupportedFormat,
                                       std::format("Unsupported image type for JPEG: {}",
                                                   static_cast<int>(image.getType())));
        }

        // Set default compression parameters
        jpeg_set_defaults(&cinfo);

        // Set quality
        jpeg_set_quality(&cinfo, quality, TRUE);

        // Start compression
        jpeg_start_compress(&cinfo, TRUE);

        // Create a buffer for one row
        int rowSize = cinfo.image_width * cinfo.input_components;
        std::vector<JSAMPLE> buffer(rowSize);
        JSAMPROW row_pointer[1] = {buffer.data()};

        // Process each scanline
        if (image.getType() == Image::Type::Grayscale) {
            // Grayscale image
            const auto& grayImage = static_cast<const GrayscaleImage&>(image);

            for (int y = 0; y < cinfo.image_height; ++y) {
                // Copy row data to buffer
                for (int x = 0; x < cinfo.image_width; ++x) {
                    auto pixelResult = grayImage.getPixel(x, y);
                    if (!pixelResult) {
                        jpeg_destroy_compress(&cinfo);
                        fclose(outfile);
                        return makeVoidErrorResult(pixelResult.error().code(),
                                                   pixelResult.error().message());
                    }

                    buffer[x] = pixelResult.value();
                }

                // Write the row
                jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }
        } else {
            // Color image
            const auto& colorImage = static_cast<const ColorImage&>(image);

            for (int y = 0; y < cinfo.image_height; ++y) {
                // Copy row data to buffer
                for (int x = 0; x < cinfo.image_width; ++x) {
                    uint8_t r, g, b, a;
                    auto pixelResult = colorImage.getPixel(x, y, r, g, b, a);
                    if (!pixelResult) {
                        jpeg_destroy_compress(&cinfo);
                        fclose(outfile);
                        return makeVoidErrorResult(pixelResult.error().code(),
                                                   pixelResult.error().message());
                    }

                    buffer[x * 3] = r;
                    buffer[x * 3 + 1] = g;
                    buffer[x * 3 + 2] = b;
                    // Alpha is ignored for JPEG
                }

                // Write the row
                jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }
        }

        // Finish compression
        jpeg_finish_compress(&cinfo);

        // Clean up
        jpeg_destroy_compress(&cinfo);
        fclose(outfile);

        return makeVoidSuccessResult();
    } catch (const std::exception& e) {
        // Close file if still open
        if (outfile) {
            fclose(outfile);
        }

        return makeVoidErrorResult(ErrorCode::InternalError,
                                   std::format("Error writing JPEG file: {}", e.what()));
    }
}

}  // namespace DIPAL

#else  // DIPAL_HAS_JPEG not defined

namespace DIPAL {

Result<std::unique_ptr<Image>> JPEGImageIO::load([[maybe_unused]]std::string_view filename) {
    return makeErrorResult<std::unique_ptr<Image>>(
        ErrorCode::UnsupportedFormat, "JPEG support is not enabled in this build of DIPAL");
}

VoidResult JPEGImageIO::save([[maybe_unused]]const Image& image, [[maybe_unused]]std::string_view filename,[[maybe_unused]] int quality) {
    return makeVoidErrorResult(ErrorCode::UnsupportedFormat,
                               "JPEG support is not enabled in this build of DIPAL");
}

}  // namespace DIPAL

#endif  // DIPAL_HAS_JPEG
