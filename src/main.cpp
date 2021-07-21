#include <iostream>
#include "TgaImage.h"
#include "tgaDrawer.h"

float edgeFunction(const Vector2f &a, const Vector2f &b, const Vector2f &c)
{
    return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x());
}

int main() {
    auto *img = new TgaImage(512, 512, TgaImage::FORMAT::RGB);
    tgaDrawer drawer(img);

    Vector2i v0 = {491, 411};
    Vector2i v1 = {148, 68};
    Vector2i v2 = {148, 411};

    TgaColor c = { 0, 128, 180 };

    drawer.triangle(v0, v1, v2, c);

    img->dump("test.tga");

    return 0;
}
