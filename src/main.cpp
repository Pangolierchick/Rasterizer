#include <iostream>
#include "TgaImage.h"

int main() {
    TgaImage img(300, 300, TgaImage::FORMAT::RGB);

    TgaColor red(0xff, 0, 0);
    TgaColor blue(128, 0, 0xff);

    for (size_t i = 50; i < 150; i++) {
        img.set(i, i, red);
        img.set(i + 20, i, blue);
    }

    img.dump("test.tga");

    return 0;
}
