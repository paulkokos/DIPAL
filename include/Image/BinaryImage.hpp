#ifndef DIPAL_BINARYIMAGE_HPP
#define DIPAL_BINARYIMAGE_HPP
#include <iostream>
#include "Image.hpp"

namespace DIPAL {
  class BinaryImage : public Image {
    public:
      //        BinaryImage(std::uint64_t *height, std::uint64_t *width, std::uint8_t *colorDepth, std::string *fileName);


      BinaryImage();
      //        std::string imShow() const override;
      //        std::string imRead(std::string FileLocation) const override;
      //        std::string imRead(std::string FileLocation std::string fileType) const override;

  };
}

#endif //DIPAL_BINARYIMAGE_HPP
