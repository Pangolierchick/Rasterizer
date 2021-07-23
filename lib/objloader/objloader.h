#pragma once

#include <vector>
#include <string>

namespace objloader {

    class Objloader;

    enum OBJLOADER_ERRORS {
        NO_ERROR,
        NO_SUCH_FILE,
        NOT_ENOUGH_VERTEX_COORDINATES,
        BAD_COORDINATE,
        UNKNOWN_TOKEN
    };

#ifdef OBJLOADER_USE_DOUBLE
    using real_t = double;
#else
    using real_t = float;
#endif

    struct real3d {
        real3d(real_t _x, real_t _y, real_t _z) : x(_x), y(_y), z(_z) {}

        real_t x;
        real_t y;
        real_t z;
    };

    struct real2d {
        real2d(real_t _x, real_t _y) : x(_x), y(_y) {}

        real_t x;
        real_t y;
    };

    struct indices {
        indices(size_t _v, size_t _vt = std::string::npos, size_t _vn = std::string::npos) : v(_v), vt(_vt), vn(_vn) {}

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

        bool isSmooth();

        std::string getName();

    private:
        std::vector<real3d> vertices;                       // v
        std::vector<real3d> texture_coord;                  // vt
        std::vector<real3d> normals;                        // vn
        std::vector<std::vector<indices>> face_elements;    // f

        std::string name; // g

        bool smooth; // s
    };

    class Objloader {
    public:
        Model load(const std::string &filename, OBJLOADER_ERRORS &error);
    };
};



