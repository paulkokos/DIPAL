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


    Header::Header() {
        std::cout << "Header Singleton created\n";
    }

    Header::~Header() {
        std::cout << "Header Singleton destroyed\n";
        delete headerPtr;
    }

}