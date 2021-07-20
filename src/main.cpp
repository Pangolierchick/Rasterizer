#include <iostream>
#include "TgaImage.h"
#include "tgaDrawer.h"

int main() {
    auto *img = new TgaImage(300, 300, TgaImage::FORMAT::RGB);
    tgaDrawer drawer(img);

    TgaColor red(0xff, 0, 0);
    TgaColor blue(128, 0, 0xff);

    drawer.line(Vector2i(50, 50), Vector2i(230, 300), blue);
    drawer.line(Vector2i(50, 250), Vector2i(230, 10), red);

    img->dump("test.tga");

    return 0;
}
