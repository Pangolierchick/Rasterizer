#pragma once

#include <vector>
#include <string>
#include "vector.h"

namespace objloader {

    class Objloader;

    enum OBJLOADER_ERRORS {
        NO_ERROR,
        NO_SUCH_FILE,
        NOT_ENOUGH_VERTEX_COORDINATES,
        BAD_COORDINATE,
        UNKNOWN_TOKEN,
        NOT_TRIANGULATED,
    };

#ifdef OBJLOADER_USE_DOUBLE
    using real_t = double;
#else
    using real_t = float;
#endif

//    struct real3d {
//        real3d(real_t _x, real_t _y, real_t _z) : x(_x), y(_y), z(_z) {}
//
//        real_t x;
//        real_t y;
//        real_t z;
//    };
//
//    struct real2d {
//        real2d(real_t _x, real_t _y) : x(_x), y(_y) {}
//
//        real_t x;
//        real_t y;
//    };

    using real2d = Vector2f;
    using real3d = Vector3f;

    struct indices {
        indices(size_t _v, size_t _vt = std::string::npos, size_t _vn = std::string::npos) : v(_v), vt(_vt), vn(_vn) {}
        indices(size_t *va): v(va[0]), vt(va[1]), vn(va[2]) {}

        size_t v;
        size_t vt;
        size_t vn;
    };

    class Model {
        friend class Objloader;

    public:
        std::vector<real3d> &getVertices();

        std::vector<real3d> &getTextureCoord();

        std::vector<real3d> &getNormals();

        std::vector<std::vector<indices>> &getFaces();

        std::vector<indices> &getFace(uint64_t ind);

        real3d getVertex(uint64_t ind);

        int getSmooth();

        std::string getName();

        void print();

        std::vector<real3d> vertices;                       // v
        std::vector<real3d> texture_coord;                  // vt
        std::vector<real3d> normals;                        // vn
        std::vector<std::vector<indices>> face_elements;    // f

        std::string name; // g

        int smooth; // s
    };

    class Objloader {
    public:
        Model load(const std::string &filename, OBJLOADER_ERRORS &error);
    };
};



