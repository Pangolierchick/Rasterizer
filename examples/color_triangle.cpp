#include "TgaImage.h"
#include "tgaDrawer.h"

float edgeFunction(const Vector2f &a, const Vector2f &b, const Vector2f &c)
{
    return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x());
}

int main() {
    auto *img = new TgaImage(512, 512, TgaImage::FORMAT::RGB);
    tgaDrawer drawer(img);

    Vector2f v0 = {491.407, 411.407};
    Vector2f v1 = {148.593, 68.5928};
    Vector2f v2 = {148.593, 411.407};

    Vector3f c0 = {1.0f, 0.0f, 0.0f};
    Vector3f c1 = {0.0f, 1.0f, 0.0f};
    Vector3f c2 = {0.0f, 0.0f, 1.0f};

    float area = edgeFunction(v0, v1, v2);

    for (uint32_t j = 0; j < img->heigth(); ++j) {
        for (uint32_t i = 0; i < img->width(); ++i) {
            Vector2f p = {i + 0.5f, j + 0.5f};

            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;

                float r = w0 * c0.x() + w1 * c1.x() + w2 * c2.x();
                float g = w0 * c0.y() + w1 * c1.y() + w2 * c2.y();
                float b = w0 * c0.z() + w1 * c1.z() + w2 * c2.z();

                img->set(j, i, TgaColor(r * 0xff, g * 0xff, b * 0xff));
            }
        }
    }


    img->dump("test.tga");

    return 0;
}
