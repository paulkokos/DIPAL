//
// Created by paulkokos on 1/27/2024.
//

#include "../../include/Image/Header.h"


void Header::print() {

}

Header *Header::getInstance() {

    return nullptr;
}

void Header::setValues() {

}

Header::Header() {

}

unsigned int Header::getWidth() const {
    return width;
}

void Header::setWidth(unsigned int width) {
    Header::width = width;
}

unsigned int Header::getHeight() const {
    return height;
}

void Header::setHeight(unsigned int height) {
    Header::height = height;
}
