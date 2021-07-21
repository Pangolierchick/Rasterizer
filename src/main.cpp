#include <iostream>
#include "TgaImage.h"
#include "tgaDrawer.h"

int main() {
    auto *img = new TgaImage(300, 300, TgaImage::FORMAT::RGB);
    tgaDrawer drawer(img);

    TgaColor red(0xff, 0, 0);
    TgaColor blue(128, 0, 0xff);

//    drawer.line(Vector2i(50, 50), Vector2i(230, 300), blue);
//    drawer.line(Vector2i(50, 250), Vector2i(230, 10), red);

    Vector2i a(30, 30);
    Vector2i b(150, 40);
    Vector2i c(20, 160);

    drawer.triangle(a, b, c, blue);

    img->dump("test.tga");

    return 0;
}
