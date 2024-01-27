#include <iostream>
#include "include/Image/Image.hpp"
//#include "Image/Image.hpp"
using namespace DIPAL;
int main() {
    auto image = new Image();
//    image->imageRead("output.bmp","bmp");
    image->imageShow();
    return 0;
}
