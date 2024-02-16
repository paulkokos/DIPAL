#include <iostream>
#include "include/image/Image.hpp"
#include "include/image/GrayscaleImage.hpp"
#include "include/Image/BinaryImage.hpp"
using namespace DIPAL;
//Header* Header::headerPtr = nullptr;
int main() {
    Image *image = new Image();
//    image
//      Header& header = Header::getHeaderInstance();
//      header.getHeader();

      Image *image1 = new GrayscaleImage();
    //auto *image = new GrayscaleImage();
    image->helloWorld();
    auto* img = new Image();
    img->helloWorld();
    //Header *header = Header::getHeaderInstance();
    return 0;
}