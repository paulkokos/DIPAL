// src/ImageProcessor/ParallelProcessor.cpp
#include "../../include/DIPAL/ImageProcessor/ParallelProcessor.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Utils/Concurrency.hpp"

#include <vector>
#include <future>
#include <format>

namespace DIPAL {

//ParallelProcessor::ParallelProcessor(size_t numThreads)
//    : ImageProcessor(), m_threadPool(numThreads), m_numThreads(numThreads) {}

Result<std::unique_ptr<Image>> ParallelProcessor::applyFilter(
    const Image& image,
    const FilterStrategy& filter
) {
    notifyProcessingStarted(filter.getName());
    notifyProgressUpdated(0.0f);

    try {
        // For small images, delegate to the base class implementation
        if (image.getWidth() * image.getHeight() < 100000) {  // Arbitrary threshold
            return ImageProcessor::applyFilter(image, filter);
        }
        
        // For larger images, divide the work among threads
        auto result = image.clone();
        int width = image.getWidth();
        int height = image.getHeight();
        
        if (width == 0 || height == 0) {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::InvalidParameter,
                "Cannot apply filter to an empty image"
            );
        }
        
        // Process the image in horizontal strips
        size_t numStrips = m_threadPool->getThreadCount();
        std::vector<std::future<Result<void>>> futures;
        
        // Different processing based on image type
        if (image.getType() == Image::Type::Grayscale) {
            auto& srcGray = static_cast<const GrayscaleImage&>(image);
            auto& dstGray = static_cast<GrayscaleImage&>(*result);
            
            for (size_t i = 0; i < numStrips; ++i) {
                int startY = static_cast<int>((height * i) / numStrips);
                int endY = static_cast<int>((height * (i + 1)) / numStrips);
                
                futures.push_back(m_threadPool->submit([&srcGray, &dstGray, startY, endY, width, &filter]() -> Result<void> {
                    // Create a temporary image for this strip
                    auto stripImage = std::make_unique<GrayscaleImage>(width, endY - startY);
                    
                    // Copy data from source to strip
                    for (int y = 0; y < endY - startY; ++y) {
                        for (int x = 0; x < width; ++x) {
                            auto pixelResult = srcGray.getPixel(x, y + startY);
                            if (!pixelResult) {
                                return makeVoidErrorResult(
                                    pixelResult.error().code(),
                                    pixelResult.error().message()
                                );
                            }
                            
                            auto setResult = stripImage->setPixel(x, y, pixelResult.value());
                            if (!setResult) {
                                return makeVoidErrorResult(
                                    setResult.error().code(),
                                    setResult.error().message()
                                );
                            }
                        }
                    }
                    
                    // Apply filter to the strip
                    auto processedResult = filter.apply(*stripImage);
                    if (!processedResult) {
                        return makeVoidErrorResult(
                            processedResult.error().code(),
                            processedResult.error().message()
                        );
                    }
                    
                    // Copy result back
                    auto& processedStrip = static_cast<GrayscaleImage&>(*processedResult.value());
                    for (int y = 0; y < endY - startY; ++y) {
                        for (int x = 0; x < width; ++x) {
                            auto pixelResult = processedStrip.getPixel(x, y);
                            if (!pixelResult) {
                                return makeVoidErrorResult(
                                    pixelResult.error().code(),
                                    pixelResult.error().message()
                                );
                            }
                            
                            auto setResult = dstGray.setPixel(x, y + startY, pixelResult.value());
                            if (!setResult) {
                                return makeVoidErrorResult(
                                    setResult.error().code(),
                                    setResult.error().message()
                                );
                            }
                        }
                    }
                    
                    return makeVoidSuccessResult();
                }));
            }
        } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            auto& srcColor = static_cast<const ColorImage&>(image);
            auto& dstColor = static_cast<ColorImage&>(*result);
            bool hasAlpha = srcColor.hasAlpha();
            
            for (size_t i = 0; i < numStrips; ++i) {
                int startY = static_cast<int>((height * i) / numStrips);
                int endY = static_cast<int>((height * (i + 1)) / numStrips);
                
                futures.push_back(m_threadPool->submit([&srcColor, &dstColor, startY, endY, width, hasAlpha, &filter]() -> Result<void> {
                    // Create a temporary image for this strip
                    auto stripImage = std::make_unique<ColorImage>(width, endY - startY, hasAlpha);
                    
                    // Copy data from source to strip
                    for (int y = 0; y < endY - startY; ++y) {
                        for (int x = 0; x < width; ++x) {
                            uint8_t r, g, b, a;
                            auto pixelResult = srcColor.getPixel(x, y + startY, r, g, b, a);
                            if (!pixelResult) {
                                return makeVoidErrorResult(
                                    pixelResult.error().code(),
                                    pixelResult.error().message()
                                );
                            }
                            
                            auto setResult = stripImage->setPixel(x, y, r, g, b, a);
                            if (!setResult) {
                                return makeVoidErrorResult(
                                    setResult.error().code(),
                                    setResult.error().message()
                                );
                            }
                        }
                    }
                    
                    // Apply filter to the strip
                    auto processedResult = filter.apply(*stripImage);
                    if (!processedResult) {
                        return makeVoidErrorResult(
                            processedResult.error().code(),
                            processedResult.error().message()
                        );
                    }
                    
                    // Copy result back
                    auto& processedStrip = static_cast<ColorImage&>(*processedResult.value());
                    for (int y = 0; y < endY - startY; ++y) {
                        for (int x = 0; x < width; ++x) {
                            uint8_t r, g, b, a;
                            auto pixelResult = processedStrip.getPixel(x, y, r, g, b, a);
                            if (!pixelResult) {
                                return makeVoidErrorResult(
                                    pixelResult.error().code(),
                                    pixelResult.error().message()
                                );
                            }
                            
                            auto setResult = dstColor.setPixel(x, y + startY, r, g, b, a);
                            if (!setResult) {
                                return makeVoidErrorResult(
                                    setResult.error().code(),
                                    setResult.error().message()
                                );
                            }
                        }
                    }
                    
                    return makeVoidSuccessResult();
                }));
            }
        } else {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type: {}", static_cast<int>(image.getType()))
            );
        }
        
        // Wait for all strips to be processed
        for (size_t i = 0; i < futures.size(); ++i) {
            auto& future = futures[i];
            auto stripResult = future.get();
            
            // Update progress after each strip is completed
            notifyProgressUpdated(static_cast<float>(i + 1) / numStrips);
            
            if (!stripResult) {
                notifyError(std::format("Filter '{}' failed on strip {}: {}", 
                                      filter.getName(), i, stripResult.error().message()));
                notifyProcessingCompleted(filter.getName(), false);
                return makeErrorResult<std::unique_ptr<Image>>(
                    stripResult.error().code(),
                    stripResult.error().message()
                );
            }
        }
        
        notifyProgressUpdated(1.0f);
        notifyProcessingCompleted(filter.getName(), true);
        return makeSuccessResult(std::move(result));
    } catch (const std::exception& e) {
        std::string errorMsg = std::format("Exception during parallel filter application: {}", e.what());
        notifyError(errorMsg);
        notifyProcessingCompleted(filter.getName(), false);
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            errorMsg
        );
    }
}



void ParallelProcessor::setThreadCount(size_t numThreads) {
    // If m_numThreads isn't available, just store the value in the ThreadPool
    m_threadPool = std::make_unique<ThreadPool>(numThreads);
}

size_t ParallelProcessor::getThreadCount() const {
    return m_threadPool ? m_threadPool->getThreadCount() : 0;
}

} // namespace DIPAL
