cmake_minimum_required(VERSION 3.19)
project(ObjLoader)

add_compile_definitions(OBJ_LOADER_DEBUG)

set(SOURCES objloader.cpp objloader.h auxilary.cpp auxilary.h)

add_library(ObjLoader STATIC ${SOURCES})