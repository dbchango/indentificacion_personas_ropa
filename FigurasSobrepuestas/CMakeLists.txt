cmake_minimum_required(VERSION 3.17)
project(FigurasSobrepuestas)

set(CMAKE_CXX_STANDARD 14)

add_executable(FigurasSobrepuestas main.cpp)

target_link_libraries(FigurasSobrepuestas -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32)