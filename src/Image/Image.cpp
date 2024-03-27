//
// Created by paulkokos on 11/27/2020.
//

#include <iostream>
#include <fstream>
#include "../../include/Image/Image.hpp"


namespace DIPAL{
//    void Image::helloWorld() {
//        header->setHeaderInstance(DefaultVariables::BITMAPINFOHEADER, DefaultVariables::WIDTH,
//                                  DefaultVariables::HEIGHT, DefaultVariables::OFFSETIMAGESTART,
//                                  DefaultVariables::BITDEPTH, DefaultVariables::SIZEOFDATA);
//
//void DIPAL::Image::setBitDepth(int bitDepth) {
//    Image::bitDepth = bitDepth;
//}
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
//
//FILE DIPAL::Image::imageRead(std::string location, std::string fileType) {
//
//    return FILE();
//}
//
//std::string DIPAL::Image::imageWrite(std::string name, std::string location, std::string fileType) {
//    return std::string();
//}
//
//void DIPAL::Image::imageShow() {
//    //image header 54 bytes
//    //image color table 1028 bytes
//    unsigned char headerOfFile[54];
//    unsigned char colorTable[1024];
//    char bufferHeader[54];
//    std::string buffer;
//    std::ifstream inStream;
//
//
//    inStream.open("../.../../etc/sails.bmp",std::ios::binary |std::ios::in | std::ios::app);
//    if (!inStream.is_open()) {
//        std::cout << "\nOpening of the file is failed\n";
//    } else {
//        std::cout << "Opening of the file is valid";
//    }
//    int length =0;
//    inStream.read(bufferHeader, 54);
//    if (inStream) {
//        inStream.seekg(0, inStream.end);
//        length = inStream.tellg();
//        inStream.seekg(0, inStream.beg);
//    } else {
//        std::cout << "File failed\n";
//    }
//    std::cout << "Length of file: " << length << "\n";
//    //Testing and verifing
////    for (auto i : buffer) {
////        bufferHeader[i];
////    }
//    std::cout << "-----Beginning of header-----\n";
//    for (int i =0; i < 54; i++) {
//        std::cout <<std::hex << (int)bufferHeader[i]<< "\n";
//    }
//    std:: cout << "------End of Header-----\n";
//
//    reinterpret_cast<std::string *> (bufferHeader);
//
////    std::cout << "Length " <<  bufferHeader.size();
//    //End of testing and verifing
//    inStream.close();
//}

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
    Image::Header::Header() {
        std::cout << "Hello from Class Header\n";
    }

    Image::Header::~Header() {
        std::cout << "Goodbye from Class Header\n";
    }

    unsigned char Image::Header::getSignatureB() const {
        return signature_B;
    }

    void Image::Header::setSignatureB(unsigned char signatureB) {
        signature_B = signatureB;
    }

    unsigned char Image::Header::getSignatureM() const {
        return signature_M;
    }

    void Image::Header::setSignatureM(unsigned char signatureM) {
        signature_M = signatureM;
    }

    uint8_t Image::Header::getReserved1() const {
        return reserved1;
    }

    void Image::Header::setReserved1(uint8_t reserved1) {
        Header::reserved1 = reserved1;
    }

    uint8_t Image::Header::getReserved2() const {
        return reserved2;
    }

    void Image::Header::setReserved2(uint8_t reserved2) {
        Header::reserved2 = reserved2;
    }

    unsigned int Image::Header::getBitmapInfoHeader() const {
        return bitmapInfoHeader;
    }

    void Image::Header::setBitmapInfoHeader(unsigned int bitmapInfoHeader) {
        Header::bitmapInfoHeader = bitmapInfoHeader;
    }

    unsigned int Image::Header::getWidth() const {
        return width;
    }

    void Image::Header::setWidth(unsigned int width) {
        Header::width = width;
    }

    unsigned int Image::Header::getHeight() const {
        return height;
    }

    void Image::Header::setHeight(unsigned int height) {
        Header::height = height;
    }

    uint8_t Image::Header::getPlanes() const {
        return planes;
    }

    void Image::Header::setPlanes(uint8_t planes) {
        Header::planes = planes;
    }

    unsigned int Image::Header::getOffsetImageStart() const {
        return offsetImageStart;
    }

    void Image::Header::setOffsetImageStart(unsigned int offsetImageStart) {
        Header::offsetImageStart = offsetImageStart;
    }

    uint8_t Image::Header::getBitDepth() const {
        return bitDepth;
    }

    void Image::Header::setBitDepth(uint8_t bitDepth) {
        Header::bitDepth = bitDepth;
    }

    unsigned int Image::Header::getFileSize() const {
        return fileSize;
    }

    void Image::Header::setFileSize(unsigned int fileSize) {
        Header::fileSize = fileSize;
    }

    unsigned int Image::Header::getCompressionType() const {
        return compressionType;
    }

    void Image::Header::setCompressionType(unsigned int compressionType) {
        Header::compressionType = compressionType;
    }

    unsigned int Image::Header::getSizeOfData() const {
        return sizeOfData;
    }

    void Image::Header::setSizeOfData(unsigned int sizeOfData) {
        Header::sizeOfData = sizeOfData;
    }

    unsigned int Image::Header::getHorizontalResolution() const {
        return horizontalResolution;
    }

    void Image::Header::setHorizontalResolution(unsigned int horizontalResolution) {
        Header::horizontalResolution = horizontalResolution;
    }

    unsigned int Image::Header::getVerticalResolution() const {
        return verticalResolution;
    }

    void Image::Header::setVerticalResolution(unsigned int verticalResolution) {
        Header::verticalResolution = verticalResolution;
    }

    unsigned int Image::Header::getNumberOfColors() const {
        return numberOfColors;
    }

    void Image::Header::setNumberOfColors(unsigned int numberOfColors) {
        Header::numberOfColors = numberOfColors;
    }

    unsigned int Image::Header::getNumberOfImportantColors() const {
        return numberOfImportantColors;
    }

    void Image::Header::setNumberOfImportantColors(unsigned int numberOfImportantColors) {
        Header::numberOfImportantColors = numberOfImportantColors;
    }


    Image::Image() {
        this->header = new Image::Header();
        std::cout << "Hello world from the class image\n";

    }
    Image::~Image() {
        delete this->header;
        std::cout << "Good bye world from the class image\n";
    }

    Image::Image(std::string locationFoFile, std::string nameOfFile) {
        this->locationOfFile = locationFoFile;
        this->nameOfFile = nameOfFile;
    }


}
