#include <algorithm>
#include <iostream>
#include "geometry.h"
#include "tgaDrawer.h"

tgaDrawer::tgaDrawer(TgaImage *image) : _pimage(image) {
    zbuffer = new float[image->width() * image->heigth()];
    std::fill_n(zbuffer, image->width() * image->heigth(), -std::numeric_limits<float>::max());
}

TgaColor tgaDrawer::getPixel(size_t x, size_t y) {
    if (_pimage)
        return _pimage->get(x, y);
}

void tgaDrawer::setPixel(size_t x, size_t y, const TgaColor &c) {
    if (_pimage)
        _pimage->set(x, y, c);
}

void tgaDrawer::line(Vector3f p1, Vector3f p2, const TgaColor &c) {
    int x0 = p1.x();
    int x1 = p2.x();

    int y0 = p1.y();
    int y1 = p2.y();

    bool steep = false;

    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int derror2 = std::abs(dy) * 2;
    int error2 = 0;

    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            setPixel(y, x, c);
        } else {
            setPixel(x, y, c);
        }

        error2 += derror2;

        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

static Vector3f barycentric(Vector3f &p0, Vector3f &p1, Vector3f &p2, Vector3f &d) {
    Vector3f u = cross(Vector3f(p2.x() - p0.x(), p1.x() - p0.x(), p0.x() - d.x()),
                       Vector3f(p2.y() - p0.y(), p1.y() - p0.y(), p0.y() - d.y()));

    if (std::abs(u.z()) < 1)
        return Vector3f(-1, 1, 1);

    return Vector3f(1.0f - (u.x() + u.y()) / u.z(), u.y() / u.z(), u.x() / u.z());
}

void tgaDrawer::triangle(Vector3f p1, Vector3f p2, Vector3f p3, TgaColor &c) {
    auto l_r = std::minmax({p1.x(), p2.x(), p3.x()});
    auto b_u = std::minmax({p1.y(), p2.y(), p3.y()});

    for (int j = b_u.first; j < b_u.second; j++) {
        for (int i = l_r.first; i < l_r.second; i++) {
            bool inside = true;
            Vector3f p(i, j, 0);
            Vector3f screen = barycentric(p1, p2, p3, p);

            if (screen.x() >= 0 && screen.y() >= 0 && screen.z() >= 0) {
                p.z() += p1.z() * screen.x();
                p.z() += p2.z() * screen.y();
                p.z() += p3.z() * screen.z();

                if (zbuffer[int(p.x() + p.y() * _pimage->width())] < p.z()) {
                    zbuffer[int(p.x() + p.y() * _pimage->width())] = p.z();
                    setPixel(i, j, c);
                }
            }
        }
    }
}

void tgaDrawer::triangle(Vector3f p1, Vector3f p2, Vector3f p3, TgaImage &texture, Vector3f texture_coord[],
                         float intensity) {
    auto l_r = std::minmax({p1.x(), p2.x(), p3.x()});
    auto b_u = std::minmax({p1.y(), p2.y(), p3.y()});

    auto t_l_r = std::minmax({texture_coord[0].x() * _pimage->width(), texture_coord[1].x() * _pimage->width(),
                              texture_coord[2].x() * _pimage->width()});
    auto t_b_u = std::minmax({texture_coord[0].y() * _pimage->heigth(), texture_coord[1].y() * _pimage->heigth(),
                              texture_coord[2].y() * _pimage->heigth()});

    for (int j = b_u.first, tj = t_b_u.first; j < b_u.second; j++, tj++) {
        for (int i = l_r.first, ti = t_l_r.first; i < l_r.second; i++, ti++) {
            Vector3f p(i, j, 0);
            Vector3f screen = barycentric(p1, p2, p3, p);

            if (screen.x() >= 0 && screen.y() >= 0 && screen.z() >= 0) {
                p.z() += p1.z() * screen.x();
                p.z() += p2.z() * screen.y();
                p.z() += p3.z() * screen.z();

                if ((zbuffer[int(p.x() + p.y() * _pimage->width())] - p.z()) < 0.01f) {
                    zbuffer[int(p.x() + p.y() * _pimage->width())] = p.z();

                    auto c = texture.get(ti, tj);
                    setPixel(i, j, c * intensity);
                }
            }
        }
    }
}

void tgaDrawer::model(objloader::Model &model, TgaImage &texture) {
    Vector3f light = {0, 0, -1};

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

        v1 = project(v1);
        v2 = project(v2);
        v3 = project(v3);

        Vector3f n = (wv3 - wv1) ^ (wv2 - wv1);
        n.normalize();

        float intensity = n * light;

        if (intensity > 0)
            triangle(v1, v2, v3, texture, tex_ver, intensity);
    }
}

Vector3f tgaDrawer::project(const Vector3f &vec) {
    std::cout << "z = " << vec.z() << "\n";
    return Vector3f(int((vec.x() + 1.0f) * _pimage->width() / 2.0f + 0.5f),
                    int((vec.y() + 1.0f) * _pimage->heigth() / 2.0f + 0.5f), vec.z());
}
