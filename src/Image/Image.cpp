#include <iostream>
#include <fstream>
#include "../../include/Image/Image.hpp"


namespace DIPAL{
    Image::Header::Header() {
        std::cout << "Hello from Class Header\n";
    }

    Image::Header::~Header() {
        std::cout << "Goodbye from Class Header\n";
    }
}
