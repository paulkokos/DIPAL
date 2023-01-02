//
// Created by paulkokos on 11/27/2020.
//

#include "Image.hpp"

int Image::getHeight() const {
    return height;
}

void Image::setHeight(int height) {
    Image::height = height;
}

int Image::getWidth() const {
    return width;
}

void Image::setWidth(int width) {
    Image::width = width;
}

const string &Image::getName() const {
    return name;
}

void Image::setName(const string &name) {
    Image::name = name;
}

int Image::getBitDepth() const {
    return bitDepth;
}

void Image::setBitDepth(int bitDepth) {
    Image::bitDepth = bitDepth;
}

unsigned int Image::getFileSize() const {
    return fileSize;
}

void Image::setFileSize(unsigned int fileSize) {
    Image::fileSize = fileSize;
}

int Image::getMinimumIntensity() const {
    return minimumIntensity;
}

void Image::setMinimumIntensity(int minimumIntensity) {
    Image::minimumIntensity = minimumIntensity;
}

int Image::getMaximumIntensity() const {
    return maximumIntensity;
}

void Image::setMaximumIntensity(int maximumIntensity) {
    Image::maximumIntensity = maximumIntensity;
}

const string &Image::getImageClass() const {
    return imageClass;
}

void Image::setImageClass(const string &imageClass) {
    Image::imageClass = imageClass;
}

const string &Image::getFormat() const {
    return format;
}

void Image::setFormat(const string &format) {
    Image::format = format;
}

const string &Image::getImageType() const {
    return imageType;
}

void Image::setImageType(const string &imageType) {
    Image::imageType = imageType;
}

const string &Image::getColorType() const {
    return colorType;
}

void Image::setColorType(const string &colorType) {
    Image::colorType = colorType;
}

const vector<int> &Image::getFormatSignature() const {
    return formatSignature;
}

void Image::setFormatSignature(const vector<int> &formatSignature) {
    Image::formatSignature = formatSignature;
}

const vector<int> &Image::getColorMap() const {
    return colorMap;
}

void Image::setColorMap(const vector<int> &colorMap) {
    Image::colorMap = colorMap;
}

const vector<int> &Image::getHistogram() const {
    return histogram;
}

void Image::setHistogram(const vector<int> &histogram) {
    Image::histogram = histogram;
}

FILE Image::imageRead(std::string location, std::string fileType) {

    return FILE();
}

std::string Image::imageWrite(std::string name, std::string location, std::string fileType) {
    return std::string();
}
