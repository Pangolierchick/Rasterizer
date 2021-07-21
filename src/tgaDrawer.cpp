#include <algorithm>
#include <iostream>
#include "tgaDrawer.h"
#include <cstdio>

TgaColor tgaDrawer::getPixel(size_t x, size_t y) {
    if (_pimage)
        return _pimage->get(x, y);
}

void tgaDrawer::setPixel(size_t x, size_t y, const TgaColor &c) {
    if (_pimage)
        _pimage->set(x, y, c);
}

bool edgeFunctionCW(const Vector2i &a, const Vector2i &b, const Vector2i &c) {
    return ((a.x() - b.x()) * (c.y() - a.y()) - (a.y() - b.y()) * (c.x() - a.x())) >= 0;
}

bool edgeFunctionCCW(const Vector2i &a, const Vector2i &b, const Vector2i &c) {
    return ((a.x() - b.x()) * (c.y() - a.y()) - (a.y() - b.y()) * (c.x() - a.x())) <= 0;
}


void tgaDrawer::line(Vector2i p1, Vector2i p2, const TgaColor &c) {
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

void tgaDrawer::triangle(Vector2i p1, Vector2i p2, Vector2i p3, TgaColor &c) {
    if (p1.y() > p2.y())
        std::swap(p1, p2);

    if (p1.y() > p3.y())
        std::swap(p1, p3);

    if (p2.y() > p3.y())
        std::swap(p2, p3);

    int left = std::min({p1.x(), p2.x(), p3.x()});
    int right = std::max({p1.x(), p2.x(), p3.x()});

    bool (*edgeFunction)(const Vector2i &, const Vector2i &, const Vector2i &);

    if (p2.x() == left) {
        edgeFunction = edgeFunctionCW;
    } else {
        edgeFunction = edgeFunctionCCW;
    }

    for (int j = p1.y(); j < p3.y(); j++) {
        for (int i = left; i < right; i++) {
            bool inside = true;
            Vector2i p(i, j);

            inside &= edgeFunction(p1, p2, p);
            inside &= edgeFunction(p2, p3, p);
            inside &= edgeFunction(p3, p1, p);

            if (inside) {
                setPixel(i, j, c);
            }
        }
    }
}
