#pragma once

#include <memory>
#include "abstractDrawer.h"
#include "objloader.h"

class tgaDrawer : public abstractDrawer {
public:
    tgaDrawer(TgaImage *image);
    ~tgaDrawer() override { delete[] zbuffer; }

    TgaColor getPixel(size_t x, size_t y) override;
    void setPixel(size_t x, size_t y, const TgaColor &c) override;

    void line(Vector3f p1, Vector3f p2, const TgaColor& c) override;
    void triangle(Vector3f p1, Vector3f p2, Vector3f p3, TgaColor &c) override;
    void triangle(Vector3f p1, Vector3f p2, Vector3f p3, TgaImage &texture, Vector3f texture_coord[], float intensity);
    void model(objloader::Model &model, TgaImage &texture);

private:
    Vector3f project(const Vector3f &vec);
    std::shared_ptr<TgaImage> _pimage;
    float *zbuffer;
};


