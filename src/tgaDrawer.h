#pragma once

#include <memory>
#include "abstractDrawer.h"

class tgaDrawer : public abstractDrawer {
public:
    tgaDrawer(TgaImage *image);
    ~tgaDrawer() override {}

    TgaColor getPixel(size_t x, size_t y) override;
    void setPixel(size_t x, size_t y, const TgaColor &c) override;

    void line(Vector3f p1, Vector3f p2, const TgaColor& c) override;
    void triangle(Vector3f p1, Vector3f p2, Vector3f p3, TgaColor &c) override;

private:
    std::shared_ptr<TgaImage> _pimage;
    float *zbuffer;
};


