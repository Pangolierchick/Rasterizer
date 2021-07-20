#pragma once

#include "TgaImage.h"
#include "vector.h"

class abstractDrawer {
public:
    abstractDrawer() {}
    virtual  ~abstractDrawer() {}

    virtual TgaColor getPixel(size_t x, size_t y) = 0;
    virtual void setPixel(size_t x, size_t y, const TgaColor &c) = 0;

    virtual void line(Vector2i p1, Vector2i p2, const TgaColor& c) = 0;

    virtual void triangle(Vector2i p1, Vector2i p2, Vector2i p3, TgaColor &c) = 0;
};


