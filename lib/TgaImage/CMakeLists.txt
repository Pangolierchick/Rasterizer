cmake_minimum_required(VERSION 3.19)
project(TgaImage)

set(SOURCES TgaImage.cpp TgaImage.h)

add_library(TgaImage STATIC ${SOURCES})