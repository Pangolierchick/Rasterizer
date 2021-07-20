#pragma once

#include <cstdint>
#include <algorithm>
#include <vector>
#include <string>
#include <exception>

#pragma pack(push, 1)
struct TGA_Header {
    std::uint8_t idlength{};
    std::uint8_t colormaptype{};
    std::uint8_t datatypecode{};
    std::uint16_t colormaporigin{};
    std::uint16_t colormaplength{};
    std::uint8_t colormapdepth{};
    std::uint16_t x_origin{};
    std::uint16_t y_origin{};
    std::uint16_t width{};
    std::uint16_t height{};
    std::uint8_t bitsperpixel{};
    std::uint8_t imagedescriptor{};
};
#pragma pack(pop)

class TgaColor {
public:
    TgaColor() = default;

    TgaColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) {
        bgra[0] = b;
        bgra[1] = g;
        bgra[2] = r;
        bgra[3] = a;
    }

    TgaColor(const uint8_t *arr, uint8_t len) {
        for (uint8_t i = 0; i < len; i++) {
            bgra[i] = arr[i];
        }

        bytespp = len;
    }

    uint8_t &operator[](uint8_t ind) { return bgra[ind]; }

    TgaColor operator*(float intensity) const {
        TgaColor res = *this;
        float clamped = std::max(0.0f, std::min(intensity, 1.0f));

        for (uint8_t i = 0; i < bytespp; i++)
            res[i] *= clamped;

        return res;
    }

    uint8_t length() const { return bytespp; }
    uint8_t *raw() { return bgra; }
    const uint8_t *raw() const { return bgra; }

private:
    uint8_t bgra[4];
    uint8_t bytespp;
};

class TgaException : public std::exception {
public:
    TgaException(std::string m) {
        error_msg = m;
    }

    const char *what() const noexcept override {
        return error_msg.c_str();
    }

protected:
    std::string error_msg;
};

class TgaImage {
public:
    enum FORMAT {
        GRAYSCALE = 1, RGB = 3, RGBA = 4
    };

    TgaImage();

    TgaImage(size_t w, size_t h, size_t bpp);

    void load(const std::string &filename);

    void dump(const std::string &filename, bool vflip=true, bool rle=true) const;

    void flipH();

    void flipV();

    void scale(size_t w, size_t h);

    TgaColor get(size_t x, size_t y) const;

    void set(size_t x, size_t y, const TgaColor &c);

    size_t width() const { return _width; }

    size_t heigth() const { return _height; }

    size_t bytespp() const { return _bytespp; }

    uint8_t *raw() { return colormap.data(); }

    void clear() { colormap.clear(); }

private:
    std::vector<uint8_t> colormap;
    size_t _width;
    size_t _height;
    size_t _bytespp;

    void loadRle(std::ifstream &in);
    void unloadRle(std::ofstream &out) const;
};


