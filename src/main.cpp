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

    TgaColor white = {0xff, 0xff, 0xff};


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

    Vector3f light = {0, 0, -1};

//    model.print();

    for (size_t i = 0; i < model.face_elements.size(); i++) {
        auto v1 = model.vertices[model.face_elements[i][0].v - 1];
        auto v2 = model.vertices[model.face_elements[i][1].v - 1];
        auto v3 = model.vertices[model.face_elements[i][2].v - 1];

        Vector3f tex_ver[] = {model.texture_coord[model.face_elements[i][0].vt - 1],
                              model.texture_coord[model.face_elements[i][1].vt - 1],
                              model.texture_coord[model.face_elements[i][2].vt - 1]};

        auto wv1 = v1;
        auto wv2 = v2;
        auto wv3 = v3;

        project(v1, 1024, 1024);
        project(v2, 1024, 1024);
        project(v3, 1024, 1024);

         Vector3f n = (wv3 - wv1) ^ (wv2 - wv1);
         n.normalize();

         float intensity = n * light;
//             auto c = TgaColor(0xff * intensity, 0xff * intensity, 0xff * intensity, 0xff);
         drawer.triangle(v1, v2, v3, texture, tex_ver, intensity);
    }

    img->dump("test.tga");

    return 0;
}
