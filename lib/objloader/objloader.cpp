#include <fstream>
#include <sstream>
#include <iostream>
#include "objloader.h"

using namespace objloader;

std::vector<real3d> Model::getVertices() {
    return vertices;
}

std::vector<real2d> Model::getTextureCoord() {
    return texture_coord;
}

std::vector<real3d> Model::getNormals() {
    return normals;
}

std::vector<std::vector<uint64_t>> Model::getFaces() {
    return face_elements;
}

std::vector<uint64_t> Model::getFace(uint64_t ind) {
    return face_elements[ind];
}

real3d Model::getVertex(uint64_t ind) {
    return vertices[ind];
}

bool Model::isSmooth() {
    return smooth;
}

std::string Model::getName() {
    return name;
}

Model Objloader::load(std::string filename, OBJLOADER_ERRORS &error) {
    std::ifstream in(filename);

    if (!in) {
        error = OBJLOADER_ERRORS::NO_SUCH_FILE;
        std::cerr << "No such file " << filename << "\n";
        return {};
    }

    std::string buff;

    while (std::getline(in, buff)) {
        
    }
}