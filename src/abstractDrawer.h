#pragma once

#include "TgaImage.h"
#include "vector.h"

class abstractDrawer {
public:
    abstractDrawer() {}
    virtual  ~abstractDrawer() {}

    virtual TgaColor getPixel(size_t x, size_t y) = 0;
    virtual void setPixel(size_t x, size_t y, const TgaColor &c) = 0;

    virtual void line(Vector3f p1, Vector3f p2, const TgaColor& c) = 0;

    virtual void triangle(Vector3f p1, Vector3f p2, Vector3f p3, TgaColor &c) = 0;
};


