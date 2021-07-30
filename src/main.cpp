#include <iostream>
#include "TgaImage.h"
#include "tgaDrawer.h"
#include "geometry.h"
#include "objloader.h"

void project(Vector3f &v, int w, int h) {
    v.x() = (v.x() + 1.0f) * (w / 2.0f + 0.5f);
    v.y() = (v.y() + 1.0f) * (h / 2.0f + 0.5f);
}

int main() {
    auto *img = new TgaImage(1024, 1024, TgaImage::FORMAT::RGB);
    tgaDrawer drawer(img);

    objloader::Objloader loader;
    objloader::OBJLOADER_ERRORS error;

    TgaImage texture;
    std::cout << "Reading\n";

    texture.load("/Users/kirill/Study/my_own_study/Rasterizer/assets/textures/head.tga");

    std::cout << "Done.\n";
    printf("Texture loaded: %zu %zu %zu\n", texture.width(), texture.heigth(), texture.bytespp());
    texture.flipV();

    auto s = clock();

    objloader::Model model = loader.load("/Users/kirill/Study/my_own_study/Rasterizer/assets/obj/head.obj", error);

    auto e = clock();
    printf("Time: %.3lf\n", (double )(e - s) / CLOCKS_PER_SEC);

    std::cout << "Error: " << error << "\n";

    drawer.model(model, texture);

    img->dump("test.tga");

    return 0;
}
