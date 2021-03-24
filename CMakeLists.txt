cmake_minimum_required(VERSION 3.16)
project(DIPAL)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(libs)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

add_library(DIPAL include/Image/Image.hpp include/Image/Image.cpp include/Segmentation/Segmentation.cpp include/Segmentation/Segmentation.hpp include/Format/Format.cpp include/Format/Format.hpp include/Compression/JPEG.cpp include/Compression/JPEG.hpp include/Compression/BMP.cpp include/Compression/BMP.hpp include/Compression/TIFF.cpp include/Compression/TIFF.hpp include/Compression/GIF.cpp include/Compression/GIF.hpp include/Compression/PNG.cpp include/Compression/PNG.hpp include/Compression/SVG.cpp include/Compression/SVG.hpp include/Math/Math.cpp include/Math/Math.hpp include/CalendarDate.cpp include/CalendarDate.h include/CalendarTime.cpp include/CalendarTime.h  include/Compression/RAW.cpp include/Compression/RAW.hpp)
add_executable(MAIN main.cpp)