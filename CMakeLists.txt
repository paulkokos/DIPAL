cmake_minimum_required(VERSION 3.16)
project(DIPAL)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(libs)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

#add_library(DIPAL include/Color/Color.cpp include/Color/Color.hpp include/Actions.cpp include/Actions.hpp include/Image/Image.hpp include/Image/Image.cpp include/Segmentation/Segmentation.cpp include/Segmentation/Segmentation.hpp include/Format/Format.cpp include/Format/Format.hpp include/Compression/JPEG.cpp include/Compression/JPEG.hpp include/Compression/BMP.cpp include/Compression/BMP.hpp include/Compression/TIFF.cpp include/Compression/TIFF.hpp include/Compression/GIF.cpp include/Compression/GIF.hpp include/Compression/PNG.cpp include/Compression/PNG.hpp include/Compression/SVG.cpp include/Compression/SVG.hpp include/Math/Math.cpp include/Math/Math.hpp include/CalendarDate.cpp include/CalendarDate.h include/CalendarTime.cpp include/CalendarTime.h  include/Compression/RAW.cpp include/Compression/RAW.hpp include/Image/BinaryImage.cpp include/Image/BinaryImage.hpp include/Image/ColorImage.cpp include/Image/ColorImage.h include/Transformation/Geometric/Translation.cpp include/Transformation/Geometric/Translation.hpp include/Transformation/Geometric/Rotation.cpp include/Transformation/Geometric/Rotation.hpp include/Transformation/Geometric/Scaling.cpp include/Transformation/Geometric/Scaling.hpp include/Transformation/Geometric/Shear.cpp include/Transformation/Geometric/Shear.hpp include/Transformation/Geometric/Reflection.cpp include/Transformation/Geometric/Reflection.hpp include/Classification/Classification.cpp include/Classification/Classification.hpp include/Math/Vectors.cpp include/Math/Vectors.hpp include/Math/LinearAlgebra.cpp include/Math/LinearAlgebra.hpp include/Color/RGB.cpp include/Color/RGB.hpp include/Color/SRGB.cpp include/Color/SRGB.hpp include/Color/CMY.cpp include/Color/CMY.hpp include/Color/YIQ.cpp include/Color/YIQ.hpp include/Color/HSI.cpp include/Color/HSI.hpp include/Color/HSV.cpp include/Color/HSV.hpp include/Color/HLS.cpp include/Color/HLS.h include/Color/YCBCR.cpp include/Color/YCBCR.hpp include/Color/CIELAB.cpp include/Color/CIELAB.hpp include/Color/HMMD.cpp include/Color/HMMD.hpp)
target_link_libraries(Classification Color)
#target_link_libraries(Color)

add_executable(MAIN main.cpp include/DIPAL.cpp include/DIPAL.hpp example.cpp)