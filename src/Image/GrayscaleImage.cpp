//
// Created by paulkokos on 23/1/21.
//

#include "../../../DIPAL/include/Image/GrayscaleImage.hpp"
//constructors

namespace DIPAL {
    DIPAL::GrayscaleImage::GrayscaleImage() {
        std::cout << "Hello world form GrayScale Class\n";

    }

    DIPAL::ImageCreator DIPAL::GrayscaleImage::RGB2Gray(DIPAL::ColorImage image) {
        return DIPAL::ImageCreator();
    }

    void DIPAL::GrayscaleImage::imRead(std::string fileName) {

    }
}