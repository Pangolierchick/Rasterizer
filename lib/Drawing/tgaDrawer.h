#pragma once

#include <memory>
#include "abstractDrawer.h"

class tgaDrawer : public abstractDrawer {
public:
    tgaDrawer(TgaImage *image): _pimage(image) { }
     ~tgaDrawer() override {}

    TgaColor getPixel(size_t x, size_t y) override;
    void setPixel(size_t x, size_t y, TgaColor &c) override;

    void line(Vector2i p1, Vector2i p2, const TgaColor& c) override;

    void triangle(Vector2i p1, Vector2i p2, Vector3i p3, TgaColor &c) override;

private:
    std::shared_ptr<TgaImage> _pimage;
};


