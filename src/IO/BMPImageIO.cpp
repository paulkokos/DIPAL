// src/IO/BMPImageIO.cpp
#include "../../include/DIPAL/IO/BMPImageIO.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <fstream>
#include <vector>
#include <format>

namespace DIPAL {

// BMP file header structure (14 bytes)
#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t signature;       // 'BM'
    uint32_t fileSize;        // Size of the BMP file in bytes
    uint16_t reserved1;       // Reserved
    uint16_t reserved2;       // Reserved
    uint32_t dataOffset;      // Offset to pixel data
};

// BMP info header structure (40 bytes)
struct BMPInfoHeader {
    uint32_t headerSize;      // Size of this header (40 bytes)
    int32_t width;            // Image width
    int32_t height;           // Image height
    uint16_t planes;          // Number of color planes (must be 1)
    uint16_t bitsPerPixel;    // Bits per pixel
    uint32_t compression;     // Compression method
    uint32_t imageSize;       // Size of the raw bitmap data
    int32_t xPixelsPerMeter;  // Horizontal resolution
    int32_t yPixelsPerMeter;  // Vertical resolution
    uint32_t colorsUsed;      // Number of colors in the palette
    uint32_t colorsImportant; // Number of important colors
};

#pragma pack(pop)

template<typename T>
Result<T> checkPixelResult(VoidResult result, int x, int y, [[maybe_unused]] T errorValue) {
    if (!result) {
        return makeErrorResult<T>(
            result.error().code(),
            std::format("Failed to set pixel at ({}, {}): {}", 
                      x, y, result.error().message())
        );
    }
    return Result<T>{};  // Success
}

Result<std::unique_ptr<Image>> BMPImageIO::load(std::string_view filename) {
    std::ifstream file(std::string(filename), std::ios::binary);
    if (!file) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::FileNotFound,
            std::format("Failed to open file: {}", filename)
        );
    }

    try {
        // Read BMP headers
        BMPFileHeader fileHeader;
        BMPInfoHeader infoHeader;
        
        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
        
        // Validate BMP signature ('BM')
        if (fileHeader.signature != 0x4D42) {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::InvalidFormat,
                "Not a valid BMP file"
            );
        }
        
        // Check if this is a supported BMP format
        if (infoHeader.headerSize != 40 || infoHeader.compression != 0) {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                "Only uncompressed BMP with BITMAPINFOHEADER is supported"
            );
        }
        
        // Check supported color depths
        if (infoHeader.bitsPerPixel != 24 && infoHeader.bitsPerPixel != 8) {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported color depth: {} bits per pixel", infoHeader.bitsPerPixel)
            );
        }
        
        // Get dimensions (height can be negative for top-down BMPs)
        int width = infoHeader.width;
        int height = abs(infoHeader.height);
        bool topDown = infoHeader.height < 0;
        
        // Create the appropriate image
        std::unique_ptr<Image> image;
        
        if (infoHeader.bitsPerPixel == 24) {
            // 24-bit color image
            auto colorResult = ImageFactory::createColor(width, height);
            if (!colorResult) {
                return makeErrorResult<std::unique_ptr<Image>>(
                    colorResult.error().code(),
                    colorResult.error().message()
                );
            }
            image = std::move(colorResult.value());
            auto& colorImage = static_cast<ColorImage&>(*image);
            
            // Calculate row padding (BMP rows are padded to 4-byte boundaries)
            int rowSize = ((width * 3 + 3) / 4) * 4;
            std::vector<uint8_t> rowData(rowSize);
            
            // Seek to pixel data
            file.seekg(fileHeader.dataOffset);
            
            // Read pixel data
            for (int y = 0; y < height; ++y) {
                int readY = topDown ? y : (height - 1 - y);
                
                file.read(reinterpret_cast<char*>(rowData.data()), rowSize);
                
                if (file.gcount() != rowSize) {
                    return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::InvalidFormat,
                        "Failed to read complete row data"
                    );
                }
                
                for (int x = 0; x < width; ++x) {
                    // BMP stores pixels in BGR order
                    uint8_t b = rowData[x * 3];
                    uint8_t g = rowData[x * 3 + 1];
                    uint8_t r = rowData[x * 3 + 2];
                    
                    auto setResult = colorImage.setPixel(x, readY, r, g, b);
                    auto checkResult = checkPixelResult<std::unique_ptr<Image>>(setResult,x,readY,nullptr);
                    if (!checkResult) {
                      return checkResult;
                    }
                }
            }
        } else if (infoHeader.bitsPerPixel == 8) {
            // 8-bit grayscale or indexed image
            // Check if there's a color palette
            if (infoHeader.colorsUsed > 0) {
                // This is an indexed image, read color palette
                std::vector<uint32_t> palette(infoHeader.colorsUsed > 0 ? infoHeader.colorsUsed : 256);
                file.read(reinterpret_cast<char*>(palette.data()), palette.size() * 4);
                
                // Create a color image and map palette indices to colors
                auto colorResult = ImageFactory::createColor(width, height);
                if (!colorResult) {
                    return makeErrorResult<std::unique_ptr<Image>>(
                        colorResult.error().code(),
                        colorResult.error().message()
                    );
                }
                image = std::move(colorResult.value());
                auto& colorImage = static_cast<ColorImage&>(*image);
                
                // Calculate row padding
                int rowSize = ((width + 3) / 4) * 4;
                std::vector<uint8_t> rowData(rowSize);
                
                // Seek to pixel data
                file.seekg(fileHeader.dataOffset);
                
                // Read pixel data
                for (int y = 0; y < height; ++y) {
                    int readY = topDown ? y : (height - 1 - y);
                    
                    file.read(reinterpret_cast<char*>(rowData.data()), rowSize);
                    
                    if (file.gcount() != rowSize) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            ErrorCode::InvalidFormat,
                            "Failed to read complete row data"
                        );
                    }
                    
                    for (int x = 0; x < width; ++x) {
                        uint8_t index = rowData[x];
                        if (index >= palette.size()) {
                            index = 0;  // Use the first color if index is out of bounds
                        }
                        
                        // Extract RGB from palette (stored as BGR0)
                        uint32_t color = palette[index];
                        uint8_t b = color & 0xFF;
                        uint8_t g = (color >> 8) & 0xFF;
                        uint8_t r = (color >> 16) & 0xFF;
                        
                        //colorImage.setPixel(x, readY, r, g, b);
                        auto setResult = colorImage.setPixel(x, readY, r, g, b);
                        auto checkResult = checkPixelResult<std::unique_ptr<Image>>(setResult, x, readY, nullptr);
                        if (!checkResult) {
                          return checkResult;  // Propagate the error
                        }
                    }
                }
            } else {
                // Treat as grayscale
                auto grayResult = ImageFactory::createGrayscale(width, height);
                if (!grayResult) {
                    return makeErrorResult<std::unique_ptr<Image>>(
                        grayResult.error().code(),
                        grayResult.error().message()
                    );
                }
                image = std::move(grayResult.value());
                auto& grayImage = static_cast<GrayscaleImage&>(*image);
                
                // Calculate row padding
                int rowSize = ((width + 3) / 4) * 4;
                std::vector<uint8_t> rowData(rowSize);
                
                // Seek to pixel data
                file.seekg(fileHeader.dataOffset);
                
                // Read pixel data
                for (int y = 0; y < height; ++y) {
                    int readY = topDown ? y : (height - 1 - y);
                    
                    file.read(reinterpret_cast<char*>(rowData.data()), rowSize);
                    
                    if (file.gcount() != rowSize) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            ErrorCode::InvalidFormat,
                            "Failed to read complete row data"
                        );
                    }
                    
                    for (int x = 0; x < width; ++x) {
                        auto setResult = grayImage.setPixel(x, readY, rowData[x]);
                        if (!setResult) {
                          return makeErrorResult<std::unique_ptr<Image>>(
                            setResult.error().code(),
                            std::format("Failed to set pixel at ({}, {}): {}", 
                            x, readY, setResult.error().message())
                          );
                        }
                    }
                }
            }
        }
        
        return makeSuccessResult(std::move(image));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InternalError,
            std::format("Error reading BMP file: {}", e.what())
        );
    }
}

VoidResult BMPImageIO::save(const Image& image, std::string_view filename) {
    std::ofstream file(std::string(filename), std::ios::binary);
    if (!file) {
        return makeVoidErrorResult(
            ErrorCode::FileAccessDenied,
            std::format("Failed to create file: {}", filename)
        );
    }

    try {
        int width = image.getWidth();
        int height = image.getHeight();
        
        // Calculate row padding (BMP rows are padded to 4-byte boundaries)
        int bytesPerPixel = (image.getType() == Image::Type::Grayscale) ? 1 : 3;
        int rowSize = ((width * bytesPerPixel + 3) / 4) * 4;
        [[maybe_unused]] int paddingSize = rowSize - (width * bytesPerPixel);
        
        // Prepare headers
        BMPFileHeader fileHeader = {};
        BMPInfoHeader infoHeader = {};
        
        // File header
        fileHeader.signature = 0x4D42;  // 'BM'
        fileHeader.reserved1 = 0;
        fileHeader.reserved2 = 0;
        
        // Info header
        infoHeader.headerSize = 40;  // BITMAPINFOHEADER
        infoHeader.width = width;
        infoHeader.height = height;  // Bottom-up (positive height)
        infoHeader.planes = 1;
        infoHeader.compression = 0;  // BI_RGB (no compression)
        infoHeader.xPixelsPerMeter = 2835;  // 72 DPI
        infoHeader.yPixelsPerMeter = 2835;  // 72 DPI
        infoHeader.colorsUsed = 0;
        infoHeader.colorsImportant = 0;
        
        if (image.getType() == Image::Type::Grayscale) {
            // 8-bit grayscale
            infoHeader.bitsPerPixel = 8;
            
            // For 8-bit BMPs, we need a color palette (grayscale)
            std::vector<uint32_t> palette(256);
            for (int i = 0; i < 256; ++i) {
                // BGR0 format
                palette[i] = (i << 16) | (i << 8) | i;
            }
            
            // Calculate file size and data offset
            fileHeader.dataOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + (256 * 4);
            infoHeader.imageSize = rowSize * height;
            fileHeader.fileSize = fileHeader.dataOffset + infoHeader.imageSize;
            
            // Write headers
            file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
            file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
            
            // Write palette
            file.write(reinterpret_cast<const char*>(palette.data()), 256 * 4);
            
            // Write pixel data
            const auto& grayImage = static_cast<const GrayscaleImage&>(image);
            std::vector<uint8_t> rowData(rowSize, 0);  // Initialize with zeros for padding
            
            for (int y = height - 1; y >= 0; --y) {  // Bottom-up
                for (int x = 0; x < width; ++x) {
                    auto result = grayImage.getPixel(x, y);
                    if (!result) {
                        return makeVoidErrorResult(
                            result.error().code(),
                            result.error().message()
                        );
                    }
                    
                    rowData[x] = result.value();
                }
                
                file.write(reinterpret_cast<const char*>(rowData.data()), rowSize);
            }
        } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            // 24-bit color (ignore alpha)
            infoHeader.bitsPerPixel = 24;
            
            // Calculate file size and data offset
            fileHeader.dataOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
            infoHeader.imageSize = rowSize * height;
            fileHeader.fileSize = fileHeader.dataOffset + infoHeader.imageSize;
            
            // Write headers
            file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
            file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
            
            // Write pixel data
            const auto& colorImage = static_cast<const ColorImage&>(image);
            std::vector<uint8_t> rowData(rowSize, 0);  // Initialize with zeros for padding
            
            for (int y = height - 1; y >= 0; --y) {  // Bottom-up
                for (int x = 0; x < width; ++x) {
                    uint8_t r, g, b, a;
                    auto result = colorImage.getPixel(x, y, r, g, b, a);
                    if (!result) {
                        return makeVoidErrorResult(
                            result.error().code(),
                            result.error().message()
                        );
                    }
                    
                    // BMP uses BGR order
                    rowData[x * 3] = b;
                    rowData[x * 3 + 1] = g;
                    rowData[x * 3 + 2] = r;
                }
                
                file.write(reinterpret_cast<const char*>(rowData.data()), rowSize);
            }
        } else {
            return makeVoidErrorResult(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type for BMP: {}", static_cast<int>(image.getType()))
            );
        }
        
        if (!file) {
            return makeVoidErrorResult(
                ErrorCode::InternalError,
                "Failed to write to file"
            );
        }
        
        return makeVoidSuccessResult();
    } catch (const std::exception& e) {
        return makeVoidErrorResult(
            ErrorCode::InternalError,
            std::format("Error writing BMP file: {}", e.what())
        );
    }
}

} // namespace DIPAL
