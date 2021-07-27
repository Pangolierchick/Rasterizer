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

int Model::getSmooth() {
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
        std::stringstream ss(buff);
        char symb;

        if (buff[0] == 'v') {
            ss >> symb;
            std::vector<real3d> *output_vector = nullptr;
            size_t offset = 0;

            if (buff[1] == 'n') {
                ss >> symb;
                output_vector = &model.normals;
                offset = 3;
            } else if (buff[1] == 't') {
                ss >> symb;
                output_vector = &model.texture_coord;
                offset = 3;
            } else if (buff[1] == ' ') {
                output_vector = &model.vertices;
                offset = 2;
            } else {
                error = OBJLOADER_ERRORS::UNKNOWN_TOKEN;
                return {};
            }

            real_t nums[3];

            for (float & num : nums) {
                ss >> num;

                if (!ss) {
                    error = OBJLOADER_ERRORS::NOT_ENOUGH_VERTEX_COORDINATES;
                    return {};
                }
            }

            output_vector->emplace_back(nums[0], nums[1], nums[2]);

        } else if (startsWith(buff, "f ")) {
            // Only triangulated models are supported
            ss >> symb;

            size_t vtn[3];
            std::vector<indices> ind;
            ind.reserve(3);

            std::string faces = buff.substr(2);

            trimString(faces);

            std::vector<std::string> coords = splitString(faces);

            int count = 0;

            for (auto &coord : coords) {
                count++;

                auto f = splitString(coord, "/");
                size_t vertices[] = { std::string::npos, std::string::npos, std::string::npos };

                for (size_t i = 0; i < f.size(); i++) {
                    try {
                        vertices[i] = std::stoul(f[i]);
                    } catch (std::exception &e) {}
                }

                ind.emplace_back(vertices);
            }

            model.face_elements.push_back(ind);

            if (count > 3) {
                error = OBJLOADER_ERRORS::NOT_TRIANGULATED;
                return {};
            }
        } else if (startsWith(buff, "g ")) {
            if (buff.length() > 2)
                model.name = buff.substr(2);
        } else if (startsWith(buff, "s ")) {
            try {
                model.smooth = std::stoi(buff.substr(2));
            } catch (std::exception &e) {
                model.smooth = 0;
            }
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

void Model::print() {
    std::cout << "------------ " << "Normals" << " ------------" << "\n";

    for (auto &i : normals) {
        std::cout << i.x() << " " << i.y() << " " << i.z() << " " << "\n";
    }

    std::cout << "------------ " << "Textures" << " ------------" << "\n";

    for (auto &i : texture_coord) {
        std::cout << i.x() << " " << i.y() << " " << i.z() << " " << "\n";
    }

    std::cout << "------------ " << "Vertices" << " ------------" << "\n";

    for (auto &i : vertices) {
        std::cout << i.x() << " " << i.y() << " " << i.z() << " " << "\n";
    }


    std::cout << "------------ " << "Indices (v, vt, vn)" << " ------------" << "\n";

    for (auto &i : face_elements) {
        std::cout << "--------------group--------------\n";

        for (auto &j : i) {
            std::cout << j.v << " ";
            if (j.vt != std::string::npos)
                std::cout << j.vt << " ";
            else
                std::cout << "_" << " ";

            if (j.vn != std::string::npos)
                std::cout << j.vn;
            else
                std::cout << "_";

            std::cout << "\n";
        }
    }
}