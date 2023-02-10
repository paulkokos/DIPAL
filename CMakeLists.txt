cmake_minimum_required(VERSION 3.16)
project(DIPAL)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
#find_package(Matplot++ REQUIRED)
add_subdirectory(matplotplusplus)
#add_library(DIPAL Classification)
#target_link_libraries(Color)

add_executable(MAIN main.cpp)
#comes always after "add_executable"
target_link_libraries(MAIN PUBLIC Color matplot)
#target_link_libraries(MAIN )
