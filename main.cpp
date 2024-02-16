#include <iostream>
#include "include/Image/Image.hpp"
#include "include/image/GrayscaleImage.hpp"
#include "include/Image/BinaryImage.hpp"
using namespace DIPAL;
//Header* Header::headerPtr = nullptr;
int main() {
    Image *img = new Image();
    img->helloWorld();
    return 0;

}