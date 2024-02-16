//
// Created by paulkokos on 11/27/2020.
//

#include <iostream>
#include <fstream>
#include "../../include/Image/Image.hpp"
#include "../../include/Image/Header.hpp"
DIPAL::Header* DIPAL::Header::headerPtr = nullptr;



namespace DIPAL {
//int DIPAL::ImageCreator::getHeight() const {
//    return ;
//}
//
//void DIPAL::ImageCreator::setHeight(int height) {
//    ImageCreator::height = height;
//}
//
//int DIPAL::ImageCreator::getWidth() const {
//    return width;
//}
//
//void DIPAL::ImageCreator::setWidth(int width) {
//    ImageCreator::width = width;
//}
//
//const string &DIPAL::ImageCreator::getName() const {
//    return name;
//}
//
//void DIPAL::ImageCreator::setName(const string &name) {
//    this->name = name;
//}
//
//int DIPAL::ImageCreator::getBitDepth() const {
//    return bitDepth;
//}
//
//void DIPAL::ImageCreator::setBitDepth(int bitDepth) {
//    ImageCreator::bitDepth = bitDepth;
//}
//
//unsigned int DIPAL::ImageCreator::getFileSize() const {
//    return fileSize;
//}
//
//void DIPAL::ImageCreator::setFileSize(unsigned int fileSize) {
//    ImageCreator::fileSize = fileSize;
//}
//
//int DIPAL::ImageCreator::getMinimumIntensity() const {
//    return minimumIntensity;
//}
//
//void DIPAL::ImageCreator::setMinimumIntensity(int minimumIntensity) {
//    ImageCreator::minimumIntensity = minimumIntensity;
//}
//
//int DIPAL::ImageCreator::getMaximumIntensity() const {
//    return maximumIntensity;
//}
//
//void DIPAL::ImageCreator::setMaximumIntensity(int maximumIntensity) {
//    DIPAL::ImageCreator::maximumIntensity = maximumIntensity;
//}
//
//const string &DIPAL::ImageCreator::getImageClass() const {
//    return imageClass;
//}
//
//void DIPAL::ImageCreator::setImageClass(const string &imageClass) {
//    ImageCreator::imageClass = imageClass;
//}
//
//const string &DIPAL::ImageCreator::getFormat() const {
//    return format;
//}
//
//void DIPAL::ImageCreator::setFormat(const string &format) {
//    ImageCreator::format = format;
//}
//
//const string &DIPAL::ImageCreator::getImageType() const {
//    return imageType;
//}
//
//void DIPAL::ImageCreator::setImageType(const string &imageType) {
//    ImageCreator::imageType = imageType;
//}
//
//const string &DIPAL::ImageCreator::getColorType() const {
//    return colorType;
//}
//
//void DIPAL::ImageCreator::setColorType(const string &colorType) {
//    ImageCreator::colorType = colorType;
//}
//
//const vector<int> &DIPAL::ImageCreator::getFormatSignature() const {
//    return formatSignature;
//}
//
//void DIPAL::ImageCreator::setFormatSignature(const vector<int> &formatSignature) {
//    ImageCreator::formatSignature = formatSignature;
//}
//
//const vector<int> &DIPAL::ImageCreator::getColorMap() const {
//    return colorMap;
//}
//
//void DIPAL::ImageCreator::setColorMap(const vector<int> &colorMap) {
//    ImageCreator::colorMap = colorMap;
//}
//
//const vector<int> &DIPAL::ImageCreator::getHistogram() const {
//    return histogram;
//}
//
//void DIPAL::ImageCreator::setHistogram(const vector<int> &histogram) {
//    ImageCreator::histogram = histogram;
//}

//FILE DIPAL::ImageCreator::imageRead(std::string location, std::string fileType) {
//
//    return FILE();
//}

//std::string DIPAL::ImageCreator::imageWrite(std::string name, std::string location, std::string fileType) {
//    return std::string();
//}

//friend void DIPAL::ImageCreator::imageShow() {
//    //image header 54 bytes
//    //image color table 1028 bytes
//    unsigned char headerOfFile[54];
//    unsigned char colorTable[1024];
//    char bufferHeader[54];
//    std::string buffer;
//    std::ifstream inStream;
//
//    inStream.open("../.../../etc/sails.bmp",std::ios::binary |std::ios::in | std::ios::app);
//    if (!inStream.is_open()) {
//        std::cout << "\nOpening of the file is failed\n";
//    } else {
//        std::cout << "Opening of the file is valid";
//    }
//    inStream.read(bufferHeader, 54);
//
//    //Testing and verifing
////    for (auto i : buffer) {
////        bufferHeader[i];
////    }
//    for (int i =0; i < 54; i++) {
//        std::cout <<std::hex << (int)bufferHeader[i]<< " ";
//    }
//
//    reinterpret_cast<std::string *> (bufferHeader);
//
////    std::cout << "Length " <<  bufferHeader.size();
//    //End of testing and verifing
//    inStream.close();
//}

//DIPAL::ImageCreator::ImageCreator() {
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

    ImageCreator::ImageCreator() {
        std::cout << "Hello world from the class image\n";

    }

    ImageCreator::~ImageCreator() {

        std::cout << "Good bye world from the class image\n";

    }

    void ImageCreator::helloWorld() {
//    header->setHeaderInstance(DefaultVariables::BITMAPINFOHEADER,DefaultVariables::WIDTH,
//                              DefaultVariables::HEIGHT,DefaultVariables::OFFSETIMAGESTART,
//                              DefaultVariables::BITDEPTH,DefaultVariables::SIZEOFDATA);

        std::cout << "Hello world from the class image\n";
    }

    void ImageCreator::imShow() {

    }


}