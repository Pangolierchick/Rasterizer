#pragma once

#include "TgaImage.h"
#include "vector.h"

class abstractDrawer {
public:
    abstractDrawer() {}
    virtual  ~abstractDrawer() = 0;

    virtual TgaColor getPixel(size_t x, size_t y) = 0;
    virtual void setPixel(size_t x, size_t y, TgaColor &c) = 0;

    virtual void line(Vector2i p1, Vector2i p2, const TgaColor& c) = 0;

    virtual void triangle(Vector2i p1, Vector2i p2, Vector3i p3, TgaColor &c) = 0;
};


