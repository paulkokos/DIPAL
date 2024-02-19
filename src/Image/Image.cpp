//
// Created by paulkokos on 11/27/2020.
//

#include <iostream>
#include <fstream>
#include "../../include/Image/Image.hpp"
#include "../../include/Image/Header.hpp"

namespace DIPAL {

    Header *Header::headerPtr = nullptr;

    Image::Image() {
        std::cout << "Hello world from the class image\n";

    }

    Image::~Image() {

        std::cout << "Good bye world from the class image\n";

    }



//    void Image::helloWorld() {
//        header->setHeaderInstance(DefaultVariables::BITMAPINFOHEADER, DefaultVariables::WIDTH,
//                                  DefaultVariables::HEIGHT, DefaultVariables::OFFSETIMAGESTART,
//                                  DefaultVariables::BITDEPTH, DefaultVariables::SIZEOFDATA);
//
//        std::cout << "Hello world from the class image\n";
//    }





//
//unsigned int DIPAL::Image::getFileSize() const {
//    return fileSize;
//}
//
//void DIPAL::Image::setFileSize(unsigned int fileSize) {
//    Image::fileSize = fileSize;
//}
//
//int DIPAL::Image::getMinimumIntensity() const {
//    return minimumIntensity;
//}
//
//void DIPAL::Image::setMinimumIntensity(int minimumIntensity) {
//    Image::minimumIntensity = minimumIntensity;
//}
//
//int DIPAL::Image::getMaximumIntensity() const {
//    return maximumIntensity;
//}
//
//void DIPAL::Image::setMaximumIntensity(int maximumIntensity) {
//    DIPAL::Image::maximumIntensity = maximumIntensity;
//}
//
//const string &DIPAL::Image::getImageClass() const {
//    return imageClass;
//}
//
//void DIPAL::Image::setImageClass(const string &imageClass) {
//    Image::imageClass = imageClass;
//}
//
//const string &DIPAL::Image::getFormat() const {
//    return format;
//}
//
//void DIPAL::Image::setFormat(const string &format) {
//    Image::format = format;
//}
//
//const string &DIPAL::Image::getImageType() const {
//    return imageType;
//}
//
//void DIPAL::Image::setImageType(const string &imageType) {
//    Image::imageType = imageType;
//}
//
//const string &DIPAL::Image::getColorType() const {
//    return colorType;
//}
//
//void DIPAL::Image::setColorType(const string &colorType) {
//    Image::colorType = colorType;
//}
//
//const vector<int> &DIPAL::Image::getFormatSignature() const {
//    return formatSignature;
//}
//
//void DIPAL::Image::setFormatSignature(const vector<int> &formatSignature) {
//    Image::formatSignature = formatSignature;
//}
//
//const vector<int> &DIPAL::Image::getColorMap() const {
//    return colorMap;
//}
//
//void DIPAL::Image::setColorMap(const vector<int> &colorMap) {
//    Image::colorMap = colorMap;
//}
//
//const vector<int> &DIPAL::Image::getHistogram() const {
//    return histogram;
//}
//
//void DIPAL::Image::setHistogram(const vector<int> &histogram) {
//    Image::histogram = histogram;
//}

//FILE DIPAL::Image::imageRead(std::string location, std::string fileType) {
//
//    return FILE();
//}

//std::string DIPAL::Image::imageWrite(std::string name, std::string location, std::string fileType) {
//    return std::string();
//}

void DIPAL::Image::imageShow() {
    //image header 54 bytes
    //image color table 1028 bytes
    unsigned char headerOfFile[54];
    unsigned char colorTable[1024];
    char bufferHeader[54];
    std::string buffer;
    std::ifstream inStream;

    inStream.open("../.../../etc/sails.bmp",std::ios::binary |std::ios::in | std::ios::app);
    if (!inStream.is_open()) {
        std::cout << "\nOpening of the file is failed\n";
    } else {
        std::cout << "Opening of the file is valid";
    }
    int length =0;
    inStream.read(bufferHeader, 54);
    if (inStream) {
        inStream.seekg(0, inStream.end);
        length = inStream.tellg();
        inStream.seekg(0, inStream.beg);
    } else {
        std::cout << "File failed\n";
    }
    std::cout << "Length of file: " << length << "\n";
    //Testing and verifing
//    for (auto i : buffer) {
//        bufferHeader[i];
//    }
    for (int i =0; i < 54; i++) {
        std::cout <<std::hex << (int)bufferHeader[i]<< "\n";
    }

    reinterpret_cast<std::string *> (bufferHeader);

//    std::cout << "Length " <<  bufferHeader.size();
    //End of testing and verifing
    inStream.close();
}

//DIPAL::Image::Image() {
//    this->header =  Header::getInstance();
//    height =0;
//    width = 0;
//    name = "noname";
//    bitDepth = 0;
//    fileSize = 0;
//    minimumIntensity = 0;
//    maximumIntensity = 0;
//    imageClass = "";
//    format = "";
//    imageType = "";
//    colorType = "";
//    formatSignature.clear();
//    colorMap.clear();
//    colorMap.clear();
//    histogram.clear();
//}


}
