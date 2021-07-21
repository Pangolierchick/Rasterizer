#include <iostream>
#include "TgaImage.h"
#include "tgaDrawer.h"
#include "geometry.h"

int main() {
    auto *img = new TgaImage(1024, 1024, TgaImage::FORMAT::RGB);
    tgaDrawer drawer(img);

    Vector3f v1 = {80, 80, 0};
    Vector3f v2 = {30, 100, 0};
    Vector3f v3 = {260, 230, 0};

    TgaColor r(0xff, 0, 0);

    drawer.triangle(v2, v3, v1, r);

    img->dump("test.tga");

    return 0;
}
