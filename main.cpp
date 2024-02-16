#include <iostream>
#include "include/Image/Image.hpp"
#include "include/image/GrayscaleImage.hpp"
#include "include/Image/BinaryImage.hpp"
using namespace DIPAL;
//Header* Header::headerPtr = nullptr;
int main() {
    Image *img = new BinaryImage();
    img->imShow();
//    ImageCreator* img1 = new BinaryImage();
    return 0;

}