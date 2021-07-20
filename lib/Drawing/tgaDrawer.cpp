#include "tgaDrawer.h"

TgaColor tgaDrawer::getPixel(size_t x, size_t y) {
    if (_pimage)
        return _pimage->get(x, y);
}

void tgaDrawer::setPixel(size_t x, size_t y, TgaColor &c) {
    if (_pimage)
        _pimage->set(x, y, c);
}

void line(Vector2i p1, Vector2i p2, const TgaColor& c) {
    
}

void triangle(Vector2i p1, Vector2i p2, Vector3i p3, TgaColor &c) {

}
