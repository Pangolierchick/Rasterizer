cmake_minimum_required(VERSION 3.19)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra)

include_directories(./lib/vector)
include_directories(./lib/TgaImage)
include_directories(./lib/Drawing)

add_subdirectory(./lib/TgaImage)
add_subdirectory(./lib/Drawing)



set(SOURCE_DIR ./src)
set(SOURCES
        ${SOURCE_DIR}/main.cpp
        )

add_executable(Rasterizer ${SOURCES})

target_link_libraries(Rasterizer TgaImage Drawing)