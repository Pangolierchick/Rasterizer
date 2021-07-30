#include "objloader.h"
#include <iostream>
#include <vector>
#include <string>

#define OBJ_LOADER_DEBUG

using namespace objloader;

void print_real3d(const char *name, std::vector<real3d> &v) {
    std::cout << "------------ " << name << " ------------" << "\n";

    for (auto &i : v) {
        std::cout << i.x << " " << i.y << " " << i.z << " " << "\n";
    }
}

void print_indices(const char *name, std::vector<std::vector<indices>> &v) {
    std::cout << "------------ " << name << " ------------" << "\n";

    for (auto &i : v) {
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

int main() {
    Objloader loader;

    OBJLOADER_ERRORS error = NO_ERROR;

    auto s = clock();

    Model model = loader.load("test1.obj", error);

    auto e = clock();

    printf("Time: %.3lf\n", (double )(e - s) / CLOCKS_PER_SEC);
//
//    print_real3d("Normals", model.getNormals());
//    print_real3d("Texture", model.getTextureCoord());
//    print_real3d("Vertices", model.getVertices());
//    print_indices("Indices (v, vt, vn)", model.getFaces());

    std::cout << "Error: " << error << "\n";
}

