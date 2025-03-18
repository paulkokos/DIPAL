#ifndef DIPAL_HPP
#define DIPAL_HPP
#include <cstddef>
#include <expected>
#include <filesystem>
#include <format>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace DIPAL {
class ImageData;
struct ImageMetadata;

/**
 * @class Image
 * @brief Base Class for all image types in the DIPAL Library
 */

class Image {
public:
  // Modern contructor using std::filesystem
  Image();
  explicit Image(const std::filesystem::path &imagePath);

  // Rule of five implementation
  virtual ~Image() = default;
  Image(const Image &other);
  Image(Image &&other) noexcept;
  Image &operator=(const Image &other);
  Image &operator=(Image &&other) noexcept;

  // Image operations
  virtual std::expected<bool, std::string>
  load(const std::filesystem::path &path);
  virtual std::expected<bool, std::string>
  load(const std::filesystem::path &path) const;
  virtual void display() const;

  // Modern getters (potentialy using C++23s explicit object parameters)
  [[nodiscard]] auto getWidth() const -> std::size_t;
  [[nodiscard]] auto getHeight() const -> std::size_t;
  [[nodiscard]] auto getChannels() const -> std::size_t;
  [[nodiscard]] auto getBitDepth() const -> std::size_t;

  // Create a view into the image data (using std::span)
  [[nodiscard]] auto data() -> std::span<std::byte>;
  [[nodiscard]] auto data() const -> std::span<const std::byte>;

  // Image info and metadata
  [[nodiscard]] auto getMetadata() const -> const ImageMetadata &;
  [[nodiscard]] auto getFilePath() const -> const std::filesystem::path &;

  // Factory method to create appropriate image type from file
  static std::expected<std::unique_ptr<Image>, std::string>
  create(const std::filesystem::path &path);

protected:
  // Implementation using the PIMPL idiom
  std::unique_ptr<ImageData> m_data;
  std::filesystem::path mm_filePath;

  // protected utility methods

  virtual void initializeData(std::size_t width, std::size_t height,
                              std::size_t channels, std::size_t getBitDepth);
};
} // namespace DIPAL
#endif
