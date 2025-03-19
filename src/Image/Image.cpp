#include <iostream>
#include <fstream>
#include "../../include/Image/Image.hpp"


namespace DIPAL{
  //Properties of class Image
  Image::Image(){
    std::cout << "Hello world from class Image\n";
    header = new Header();
  }

  Image::~Image() {
    delete header;
    std::cout << "Good bye world from the class image\n";
  }
  void Image::headerHelloWorld() {
    header->helloWorld();
  }
  //Properties of class Header
  Image::Header::Header() {
    std::cout << "Header just created\n";
    //Header header1;
    //std::cout << "Hello world from the class image\n";
  }
  void Image::Header::helloWorld(){
    std::cout << "Hello world\n";
  }
  Image::Header::~Header() {
    std::cout << "Good bye from Header\n";
  }
}
