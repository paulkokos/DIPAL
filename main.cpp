#include "include/Image/Image.hpp"
using namespace DIPAL;
int main() {
  Image *img = new Image();
  img->helloWorld();
  img->headerHelloWorld();
  delete img;
  return 0;
}
