#include <algorithm>
#include "geometry.h"
#include "tgaDrawer.h"

TgaColor tgaDrawer::getPixel(size_t x, size_t y) {
    if (_pimage)
        return _pimage->get(x, y);
}

void tgaDrawer::setPixel(size_t x, size_t y, const TgaColor &c) {
    if (_pimage)
        _pimage->set(x, y, c);
}

void tgaDrawer::line(Vector3f p1, Vector3f p2, const TgaColor &c) {
    int x0 = p1.x();
    int x1 = p2.x();

    int y0 = p1.y();
    int y1 = p2.y();

    bool steep = false;

    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int derror2 = std::abs(dy) * 2;
    int error2 = 0;

    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            setPixel(y, x, c);
        } else {
            setPixel(x, y, c);
        }

        error2 += derror2;

        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

static Vector3f barycentric(Vector3f &p0, Vector3f &p1, Vector3f &p2, Vector3f &d) {
    Vector3f u = cross(Vector3f(p2.x() - p0.x(), p1.x() - p0.x(), p0.x() - d.x()),
                       Vector3f(p2.y() - p0.y(), p1.y() - p0.y(), p0.y() - d.y()));

    if (std::abs(u.z()) < 1)
        return Vector3f(-1, 1, 1);

    return Vector3f(1.0f - (u.x() + u.y()) / u.z(), u.y() / u.z(), u.x() / u.z());
}

void tgaDrawer::triangle(Vector3f p1, Vector3f p2, Vector3f p3, TgaColor &c) {
    auto l_r = std::minmax({p1.x(), p2.x(), p3.x()});
    auto b_u = std::minmax({p1.y(), p2.y(), p3.y()});

    for (int j = b_u.first; j < b_u.second; j++) {
        for (int i = l_r.first; i < l_r.second; i++) {
            bool inside = true;
            Vector3f p(i, j);
            Vector3f screen = barycentric(p1, p2, p3, p);

            if (screen.x() >= 0 && screen.y() >= 0 && screen.z() >= 0) {
                setPixel(i, j, c);
            }
        }
    }
}
