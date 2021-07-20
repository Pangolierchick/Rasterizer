cmake_minimum_required(VERSION 3.19)
project(Drawing)

set(SOURCES abstractDrawer.h TgaDrawer.cpp TgaDrawer.h)

add_library(Drawing STATIC ${SOURCES})