#pragma once

#include <vector>
#include <string>

namespace objloader {

    using real_t = float;

    struct real3d {
        real_t x;
        real_t y;
        real_t z;
    };

    struct real2d {
        real_t x;
        real_t y;
    };

    class Model {
    public:
        std::vector<real3d> getVertices();
        std::vector<real2d> getTextureCoord();
        std::vector<real3d> getNormals();
        std::vector<std::vector<uint64_t>> getFaceElems();

        std::vector<uint64_t> getFace(uint64_t ind);
        real3d getVertex(uint64_t ind);

    private:
        std::vector<real3d> vertices;                       // v
        std::vector<real2d> texture_coord;                  // vt
        std::vector<real3d> normals;                        // vn
        std::vector<std::vector<uint64_t>> face_elements;   // f

        std::string name; // g

        bool smooth; // s
    };

    class Objloader {
    public:
        Model load(std::string filename);
    };
};



