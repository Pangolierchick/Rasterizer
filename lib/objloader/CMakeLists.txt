cmake_minimum_required(VERSION 3.19)
project(ObjLoader)

set(SOURCES objloader.cpp objloader.h)

add_library(ObjLoader STATIC ${SOURCES})