//
// Created by paulk on 12/13/2022.
//

#include "../../../DIPAL/include/Image/BinaryImage.hpp"
namespace DIPAL {
    BinaryImage::BinaryImage() {
        std::cout << "Binary image Constructed\n";

    }
    std::string BinaryImage::imShow() const {
        return "Hello from Factory class Binary Image\n";
    }


}