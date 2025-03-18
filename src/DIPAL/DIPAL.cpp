#include "../../include/DIPAL/DIPAL.hpp"

namespace DIPAL {
/**
 * @class DIPAL
 * @brief Main class for the Digital Image Processing and Analysis Library
 *
 * This class provides a facade for the entire library, making it easy to use
 * common functionality without dealing with individual components.
 */
class DIPAL {

  // Create a DIPAL instance
  DIPAL();

  // Load an image
  std::expected<std::shared_ptr<Image>, std::string>
  loadImage(const std::filesystem::path &path);

  // Save an image
  std::expected<void, std::string>
  saveImage(const std::shared_ptr<Image> &image,
            const std::filesystem::path &path);

  // Process an image with a strategy or pipeline
  std::expected<void, std::string>
  processImage(std::shared_ptr<Image> &image,
               const std::shared_ptr<ImageProcessingStrategy> &strategy);

  std::expected<void, std::string> processImage(
      std::shared_ptr<Image> &image,
      const std::vector<std::shared_ptr<ImageProcessingStrategy>> &pipeline);

  // Create a common processing strategy
  std::shared_ptr<ImageProcessingStrategy>
  createStrategy(const std::string &strategyName,
                 const std::unordered_map<std::string, std::any> &params = {});

  // Observer management
  void addObserver(std::shared_ptr<ProcessingObserver> observer);
  void removeObserver(std::shared_ptr<ProcessingObserver> observer);

  // Utility functions
  static std::string getVersionString();
  static std::string getBuildInfo();

  // Register a custom strategy factory
  void registerStrategyFactory(
      const std::string &strategyName,
      std::function<std::shared_ptr<ImageProcessingStrategy>(
          const std::unordered_map<std::string, std::any> &)>
          factory);

private:
  // Components
  ImageProcessor m_processor;
  std::shared_ptr<ProcessingSubject> m_subject;

  // Strategy factories
  std::unordered_map<std::string,
                     std::function<std::shared_ptr<ImageProcessingStrategy>(
                         const std::unordered_map<std::string, std::any> &)>>
      m_strategyFactories;

  // Initialize default strategy factories
  void initializeStrategyFactories();
};
} // namespace DIPAL
