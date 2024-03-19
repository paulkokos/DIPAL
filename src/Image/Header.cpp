//
// Created by paulkokos on 1/27/2024.
//

#include "../../include/Image/Header.hpp"
namespace DIPAL {
    Header &Header::getHeaderInstance() {
        if (!headerPtr) {
            headerPtr = new Header();
        }
        std::cout << "Header instance created\n";
        return *headerPtr;
    }

    void DIPAL::Header::getHeader() {
        std::cout << "Hello from modification method\n";
    }


    Header::Header() {
        std::cout << "Header Singleton created\n";
    }

    Header::~Header() {
        std::cout << "Header Singleton destroyed\n";
        delete headerPtr;
    }


    void Header::setVerticalResolution(unsigned int verticalResolution) {
        headerPtr->verticalResolution = verticalResolution;
    }

    unsigned int Header::getVerticalResolution() const {
        //unreliable
        return headerPtr->verticalResolution;
    }

    void Header::setNumberOfColors(unsigned int numberOfColors) {
        headerPtr->numberOfColors = numberOfColors;
    }

    unsigned int Header::getNumberOfColors() const {
        return headerPtr->numberOfColors;
    }

    unsigned int Header::getNumberOfImportantColors() const {
        return headerPtr->numberOfImportantColors;
    }

    void Header::setNumberOfImportantColors(unsigned int numberOfImportantColors) {
        headerPtr->numberOfImportantColors = numberOfImportantColors;
    }
    unsigned int Header::getHorizontalResolution() const {
        return headerPtr->horizontalResolution;
    }

    void Header::setHorizontalResolution(unsigned int horizontalResolution) {
        headerPtr->horizontalResolution = horizontalResolution;
    }

    void Header::setSizeOfData(unsigned int sizeOfData) {
        headerPtr->sizeOfData = sizeOfData;
    }

    unsigned int Header::getSizeOfData() const {
        return headerPtr->sizeOfData;
    }

    void Header::setCompressionType(unsigned int compressionType) {
        headerPtr->compressionType = compressionType;
    }

    unsigned int Header::getCompressionType() const {
        return headerPtr->compressionType;
    }

    void Header::setFileSize(unsigned int fileSize) {
        headerPtr->fileSize = fileSize;
    }

    unsigned int Header::getFileSize() const {
        return this->fileSize;
    }

    void Header::setBitDepth(uint8_t bitDepth) {
        headerPtr->bitDepth = bitDepth;
    }

    uint8_t Header::getBitDepth() const {
        return headerPtr->bitDepth;
    }

    void Header::setOffsetImageStart(unsigned int offsetImageStart) {
        headerPtr->offsetImageStart = offsetImageStart;
    }

    unsigned int Header::getOffsetImageStart() const {
        return headerPtr->offsetImageStart;
    }

    void Header::setPlanes(uint8_t planes) {
        headerPtr->planes = planes;
    }

    uint8_t Header::getPlanes() const {
        return headerPtr->planes;
    }

    void Header::setHeight(unsigned int height) {
        headerPtr->height = height;
    }

    unsigned int Header::getHeight() const {
        return headerPtr->height;
    }


    void Header::setWidth(unsigned int width) {
        headerPtr->width = width;
    }

    unsigned int Header::getWidth() const {
        return headerPtr->width;
    }

    void Header::setBitmapInfoHeader(unsigned int bitmapInfoHeader) {
        headerPtr->bitmapInfoHeader= bitmapInfoHeader;
    }

    unsigned int Header::getBitmapInfoHeader() const {
        return headerPtr->bitmapInfoHeader;
    }

    void Header::setReserved2(uint8_t reserved2) {
        headerPtr->reserved2 = reserved2;
    }

    uint8_t Header::getReserved2() const {
        return headerPtr->reserved2;
    }

    void Header::setReserved1(uint8_t reserved1) {
        headerPtr->reserved1 = reserved1;
    }

    uint8_t Header::getReserved1() const {
        return headerPtr->reserved1;
    }

    void Header::setSignatureM(unsigned char signatureM) {
        headerPtr->signature_M = signatureM;
    }

    unsigned char Header::getSignatureM() const {
        return headerPtr->signature_M;
    }

    void Header::setSignatureB(unsigned char signatureB) {
        headerPtr->signature_B = signatureB;
    }

    unsigned char Header::getSignatureB() const {
        return headerPtr->signature_B;
    }

}