cmake_minimum_required(VERSION 3.19)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra)

include_directories(./lib/vector)
include_directories(./lib/TgaImage)

add_subdirectory(./lib/TgaImage)



set(SOURCE_DIR ./src)
set(SOURCES
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/abstractDrawer.h
        ${SOURCE_DIR}/tgaDrawer.cpp
        ${SOURCE_DIR}/tgaDrawer.h
        ${SOURCE_DIR}/geometry.cpp
        ${SOURCE_DIR}/geometry.h

        )

add_executable(Rasterizer ${SOURCES})

target_link_libraries(Rasterizer TgaImage)