//
// Created by paulkokos on 11/27/2020.
//

#include "Image.hpp"
Image::Image(){
    this->width =0;
    this->height =0;
    this->colorDepth =0;
    this->name ="";
    this->extension="";
}
Image::Image(int height, int width, int colorDepth, string name,string extension){
    this->height=height;
    this->width=width;
    this->colorDepth = colorDepth;
    this->name= name;
    this->extension = extension;
}

int Image::getHeight() const {
    return height;
}

int Image::getWidth() const {
    return width;
}

int Image::getColorDepth() const {
    return colorDepth;
}

void Image::setColorDepth(int colorDepth) {
    Image::colorDepth = colorDepth;
}

const string &Image::getName() const {
    return name;
}

const string &Image::getExtension() const {
    return extension;
}

void setWidth(){

}