#pragma once

#include <cmath>
#include <sstream>

template <typename T>
class Vector2 {
public:
    Vector2(): _x(0), _y(0) {}
    Vector2(T x, T y): _x(x), _y(y) {}

    Vector2(Vector2 &&v)  noexcept {
        _x = v._x;
        _y = v._y;
    }

    explicit Vector2(const Vector2 &v) {
        _x = v._x;
        _y = v._y;
    }

    T& x() { return _x; }
    T& y() { return _y; }

    Vector2 operator +(const Vector2 &v) { return Vector2(_x + v._x, _y + v._y); }
    Vector2 operator -(const Vector2 &v) { return Vector2(_x - v._x, _y - v._y); }

    T operator *(const Vector2 &v) { return _x * v._x + _y * v._y; }
    Vector2 operator *(T v) { return Vector2(_x * v, _y * v); }

    bool operator ==(const Vector2 &v) { return _x == v._x && _y == v._y; }
    bool operator != (const Vector2 &v) { return !(*this == v); }

    T length() { return sqrt(_x * _x + _y * _y); }
    void normalize() {
        auto l = this->length();
        _x = (_x / l); _y = (_y / l);
    }

    friend std::ostream& operator<< (std::ostream& os, const Vector2 &v) {
        std::stringstream stream;

        stream << "(" << v._x << ", " << v._y << ")";
        os.write(stream.str().c_str(), static_cast<std::streamsize>(stream.str().size() * sizeof(char)));

        return os;
    }

private:
    T _x, _y;
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2l = Vector2<long>;

template <typename T>
class Vector3 {
public:
    Vector3(): _x(0), _y(0), _z(0) {}
    Vector3(T x, T y, T z): _x(x), _y(y), _z(z) {}

    Vector3(Vector3 &&v)  noexcept {
        _x = v._x;
        _y = v._y;
        _z = v._z;
    }


    explicit Vector3(const Vector3 &v) {
        _x = v._x;
        _y = v._y;
        _z = v._z;
    }

    T& x() { return _x; }
    T& y() { return _y; }
    T& z() { return _z; }

    Vector3 operator +(const Vector3 &v) { return Vector3(_x + v._x, _y + v._y, _z + v._z); }
    Vector3 operator -(const Vector3 &v) { return Vector3(_x - v._x, _y - v._y, _z - v._z); }

    T operator *(const Vector3 &v) { return _x * v._x + _y * v._y + _z * v._z; }
    Vector3 operator *(T v) { return Vector3(_x * v, _y * v, _z * v); }

    bool operator ==(const Vector3 &v) { return _x == v._x && _y == v._y && _z == v._z; }
    bool operator != (const Vector3 &v) { return !(*this == v); }

    T length() { return sqrt(_x * _x + _y * _y + _z * _z); }
    void normalize() {
        auto l = this->length();
        _x = (_x / l); _y = (_y / l); _z = (_z / l);
    }

    friend std::ostream& operator<< (std::ostream& os, const Vector3 &v) {
        std::stringstream stream;

        stream << "(" << v._x << ", " << v._y << ", " << v._z << ")";
        os.write(stream.str().c_str(), static_cast<std::streamsize>(stream.str().size() * sizeof(char)));

        return os;
    }

private:
    T _x, _y, _z;
};

using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;
using Vector3l = Vector3<long>;