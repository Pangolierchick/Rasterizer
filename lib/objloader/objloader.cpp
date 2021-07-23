#include <fstream>
#include <sstream>
#include <iostream>
#include "objloader.h"
#include "auxilary.h"

using namespace objloader;

std::vector<real3d> &Model::getVertices() {
    return vertices;
}

std::vector<real3d> &Model::getTextureCoord() {
    return texture_coord;
}

std::vector<real3d> &Model::getNormals() {
    return normals;
}

std::vector<std::vector<indices>> &Model::getFaces() {
    return face_elements;
}

std::vector<indices> &Model::getFace(uint64_t ind) {
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

Model Objloader::load(const std::string &filename, OBJLOADER_ERRORS &error) {
    std::ifstream in(filename);
    Model model;

    if (!in) {
        error = OBJLOADER_ERRORS::NO_SUCH_FILE;
        std::cerr << "No such file " << filename << "\n";
        return {};
    }

    std::string buff;

    while (std::getline(in, buff)) {
        if (startsWith(buff, "v")) {
            std::vector<real3d> *output_vector = nullptr;
            size_t offset = 0;

            if (buff[1] == 'n') {
                output_vector = &model.normals;
                offset = 3;
            } else if (buff[1] == 't') {
                output_vector = &model.texture_coord;
                offset = 3;
            } else if (buff[1] == ' ') {
                output_vector = &model.vertices;
                offset = 2;
            } else {
                error = OBJLOADER_ERRORS::UNKNOWN_TOKEN;
                return {};
            }

            auto coords = splitString(buff.substr(offset));

            if (coords.size() < 3) {
                error = OBJLOADER_ERRORS::NOT_ENOUGH_VERTEX_COORDINATES;
                return {};
            }

            std::vector<real_t> nums;

            try {
                nums = toNumber(coords);
            } catch (std::invalid_argument &e) {

                error = OBJLOADER_ERRORS::BAD_COORDINATE;
                return {};
            }

            output_vector->emplace_back(nums[0], nums[1], nums[2]);

        } else if (startsWith(buff, "f ")) {
            std::vector<std::string> coords = splitString(buff.substr(2));

            if (coords.size() < 3) {
                error = OBJLOADER_ERRORS::NOT_ENOUGH_VERTEX_COORDINATES;
                return {};
            }

            std::vector<objloader::indices> ind;

            for (auto &i : coords) {
                std::vector<std::string> indices = splitString(i, "/");

                size_t vt = indices[1].empty() ? std::string::npos : std::stoul(indices[1]);

                size_t vn;
                if (indices.size() > 2) {
                    vn = indices[2].empty() ? std::string::npos : std::stoul(indices[2]);
                }
                else
                    vn = std::string::npos;

                ind.emplace_back(std::stoul(indices[0]), vt, vn);
            }

            model.face_elements.push_back(ind);

        } else if (startsWith(buff, "g ")) {
            model.name = buff.substr(2);
        } else if (startsWith(buff, "s ")) {
            model.smooth = buff.find('1', 2) != std::string::npos;
        } else if (startsWith(buff, "#")) {
            continue;
        }
    }

#ifdef OBJ_LOADER_DEBUG
    std::cout << "#v = " << model.vertices.size() << "\n";
    std::cout << "#vt = " << model.texture_coord.size() << "\n";
    std::cout << "#vn = " << model.normals.size() << "\n";
    std::cout << "#f = " << model.face_elements.size() << "\n";
    std::cout << "name = " << model.name << "\n";
    std::cout << "Smooth = " << model.smooth << "\n";

#endif

    error = OBJLOADER_ERRORS::NO_ERROR;
    return model;
}