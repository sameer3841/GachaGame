cmake_minimum_required(VERSION 3.16)
project(GachaGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib 5.0 REQUIRED)

add_executable(GachaGame main.cpp)
target_link_libraries(GachaGame raylib "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
