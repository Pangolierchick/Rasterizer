#include "tgaDrawer.h"

TgaColor tgaDrawer::getPixel(size_t x, size_t y) {
    if (_pimage)
        return _pimage->get(x, y);
}

void tgaDrawer::setPixel(size_t x, size_t y, const TgaColor &c) {
    if (_pimage)
        _pimage->set(x, y, c);
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

}
