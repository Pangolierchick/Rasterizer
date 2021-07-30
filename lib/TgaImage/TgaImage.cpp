#include <iostream>
#include <fstream>
#include "TgaImage.h"

TgaImage::TgaImage(size_t w, size_t h, size_t bpp) {
    _width = w;
    _height = h;
    _bytespp = bpp;

    colormap.reserve(w * h * bpp);
}

TgaImage::TgaImage() {
    _width = 0;
    _height = 0;
    _bytespp = 0;
}

void TgaImage::load(const std::string &filename) {
    std::ifstream in(filename, std::ios::binary);

    if (!in.is_open()) {
        in.close();

        std::string err("Can't open file " + filename);

        throw TgaException(err);
    }

    TGA_Header header;

    in.read(reinterpret_cast<char *>(&header), sizeof(header));

    if (!in.good()) {
        in.close();
        throw TgaException("Error occured while reading header.");
    }

    _width   = header.width;
    _height  = header.height;
    _bytespp = header.bitsperpixel >> 3;

    if (_bytespp != GRAYSCALE && _bytespp != RGB && _bytespp != RGBA) {
        in.close();
        throw TgaException("Error. Bad bytes per pixel.");
    }

    size_t bytes = _width * _height * _bytespp;

    colormap.resize(bytes);

    if (header.datatypecode == 3 || header.datatypecode == 2) {
        in.read(reinterpret_cast<char *>(colormap.data()), bytes);

        if (!in.good()) {
            in.close();
            throw TgaException("Error occured while readin");
        }
    } else if (header.datatypecode == 10 || header.datatypecode == 11) {
        loadRle(in);
    } else {
        in.close();
        throw TgaException("Unknown file type.");
    }

    if (!(header.imagedescriptor & 0x20))
        flipV();

    if (header.imagedescriptor & 0x10)
        flipH();

    in.close();
}

void TgaImage::loadRle(std::ifstream &in) {
    size_t pixelcount   = _width * _height;
    size_t currentpixel = 0;
    size_t currentbyte  = 0;

    TgaColor color{};

    do {
        uint8_t chunkheader = in.get();

        if (!in.good()) {
            throw TgaException("An error occurred while reading the data");
        }

        if (chunkheader < 128) {
            chunkheader++;

            for (uint8_t i = 0; i < chunkheader; i++) {
                in.read(reinterpret_cast<char *>(color.raw()), _bytespp);

                if (!in.good()) {
                    throw TgaException("An error occurred while reading the data");
                }

                for (size_t t = 0; t < _bytespp; t++)
                    colormap[currentbyte++] = color[t];

                currentpixel++;

                if (currentpixel > pixelcount)
                    throw TgaException("Too many pixels read.");
            }
        } else {
            chunkheader -= 127;

            in.read(reinterpret_cast<char *>(color.raw()), _bytespp);

            if (!in.good())
                throw TgaException("An error occurred while reading the header");

            for (size_t i = 0; i < chunkheader; i++) {
                for (size_t t = 0; t < _bytespp; t++)
                    colormap[currentbyte++] = color[t];

                currentpixel++;

                if (currentpixel > pixelcount)
                    throw TgaException("Too many pixels read.");
            }
        }
    } while (currentpixel < pixelcount);
}


void TgaImage::dump(const std::string &filename, bool vflip, bool rle) const {
    std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    std::uint8_t footer[18] = {'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', '.',
                               '\0'};
    std::ofstream out;

    out.open(filename, std::ios::binary);

    if (!out.is_open()) {
        out.close();
        throw TgaException("Can't open file" + filename);
    }
    TGA_Header header;
    header.bitsperpixel = _bytespp << 3;
    header.width = _width;
    header.height = _height;
    header.datatypecode = (_bytespp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
    header.imagedescriptor = vflip ? 0x00 : 0x20; // top-left or bottom-left origin
    out.write(reinterpret_cast<const char *>(&header), sizeof(header));
    if (!out.good()) {
        out.close();
        throw TgaException("Can't dump tga file");
    }
    if (!rle) {
        out.write(reinterpret_cast<const char *>(colormap.data()), _width * _height * _bytespp);
        if (!out.good()) {
            out.close();
            throw TgaException("Can't unload raw data");
        }
    } else {
        unloadRle(out);
    }

    out.write(reinterpret_cast<const char *>(developer_area_ref), sizeof(developer_area_ref));
    if (!out.good()) {
        out.close();
        throw TgaException("Can't dump tga file");
    }
    out.write(reinterpret_cast<const char *>(extension_area_ref), sizeof(extension_area_ref));
    if (!out.good()) {
        out.close();
        throw TgaException("Can't dump tga file");
    }
    out.write(reinterpret_cast<const char *>(footer), sizeof(footer));
    if (!out.good()) {
        out.close();
        throw TgaException("Can't dump tga file");
    }
    out.close();
}

void TgaImage::unloadRle(std::ofstream &out) const {
    const std::uint8_t max_chunk_length = 128;
    size_t npixels = _width * _height;
    size_t curpix = 0;

    while (curpix < npixels) {
        size_t chunkstart = curpix * _bytespp;
        size_t curbyte = curpix * _bytespp;
        std::uint8_t run_length = 1;

        bool raw = true;

        while (curpix + run_length < npixels && run_length < max_chunk_length) {
            bool succ_eq = true;

            for (int t = 0; succ_eq && t < _bytespp; t++)
                succ_eq = (colormap[curbyte + t] == colormap[curbyte + t + _bytespp]);

            curbyte += _bytespp;

            if (1 == run_length)
                raw = !succ_eq;

            if (raw && succ_eq) {
                run_length--;
                break;
            }
            if (!raw && !succ_eq)
                break;

            run_length++;
        }
        curpix += run_length;
        out.put(raw ? run_length - 1 : run_length + 127);
        if (!out.good()) {
            throw TgaException("Can't dump tga file");
        }
        out.write(reinterpret_cast<const char *>(colormap.data() + chunkstart),
                  (raw ? run_length * _bytespp : _bytespp));
        if (!out.good()) {
            throw TgaException("Can't dump tga file");
        }
    }
}

void TgaImage::scale(size_t w, size_t h) {
    if (w <= 0 || h <= 0 || colormap.empty()) return;

    std::vector<std::uint8_t> tdata(w * h * _bytespp, 0);
    size_t nscanline = 0;
    size_t oscanline = 0;
    size_t erry = 0;
    size_t nlinebytes = w * _bytespp;
    size_t olinebytes = _width * _bytespp;
    for (size_t j = 0; j < _height; j++) {
        size_t errx = _width - w;
        size_t nx = -_bytespp;
        size_t ox = -_bytespp;

        for (size_t i = 0; i < _width; i++) {
            ox += _bytespp;
            errx += w;

            while (errx >= (size_t) _width) {
                errx -= _width;
                nx += _bytespp;
                memcpy(tdata.data() + nscanline + nx, colormap.data() + oscanline + ox, _bytespp);
            }
        }
        erry += h;
        oscanline += olinebytes;
        while (erry >= (size_t) _height) {
            if (erry >= (size_t) _height << 1)
                memcpy(tdata.data() + nscanline + nlinebytes, tdata.data() + nscanline, nlinebytes);
            erry -= _height;
            nscanline += nlinebytes;
        }
    }
    colormap = tdata;
    _width = w;
    _height = h;
}

TgaColor TgaImage::get(size_t x, size_t y) const {
    if (x >= _width || y >= _height) {
        return {};
    }

    return TgaColor(colormap.data() + (x + y * _width) * _bytespp, _bytespp);
}

void TgaImage::set(size_t x, size_t y, const TgaColor &c) {
    if (x < _width || y < _height) {
        memcpy(colormap.data() + (x + y * _width) * _bytespp, c.raw(), _bytespp);
    }
}

void TgaImage::flipH() {
    if (colormap.empty())
        return;

    size_t half = _width >> 1;
    for (size_t i = 0; i < half; i++) {
        for (size_t j = 0; j < _height; j++) {
            TgaColor c1 = get(i, j);
            TgaColor c2 = get(_width - 1 - i, j);
            set(i, j, c2);
            set(_width - 1 - i, j, c1);
        }
    }
}

void TgaImage::flipV() {
    if (colormap.empty()) return;

    size_t bytes_per_line = _width * _bytespp;

    std::vector<std::uint8_t> line(bytes_per_line, 0);

    size_t half = _height >> 1;

    for (size_t j = 0; j < half; j++) {
        size_t l1 = j * bytes_per_line;
        size_t l2 = (_height - 1 - j) * bytes_per_line;
        std::copy(colormap.begin() + l1, colormap.begin() + l1 + bytes_per_line, line.begin());
        std::copy(colormap.begin() + l2, colormap.begin() + l2 + bytes_per_line, colormap.begin() + l1);
        std::copy(line.begin(), line.end(), colormap.begin() + l2);
    }
}
