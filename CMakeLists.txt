cmake_minimum_required(VERSION 3.16)
project(DIPAL)
set(CMAKE_CXX_STANDARD 17)

add_executable(DIPAL
        src/Image/Image.cpp
        src/Image/Header.cpp
        src/Image/BinaryImage.cpp
        src/Image/GrayscaleImage.cpp
        src/Image/ColorImage.cpp

        include/Image/Image.hpp
        include/Image/Header.hpp
        include/Image/BinaryImage.hpp
        include/Image/GrayscaleImage.hpp
        include/Image/ColorImage.hpp


        main.cpp
        src/Image/ImageFactory.cpp
        include/Image/ImageFactory.h
)
#add_executable(Main main.cpp)
