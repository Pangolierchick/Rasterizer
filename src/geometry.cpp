#include "geometry.h"

Vector3f cross(const Vector3f &a, const Vector3f &b) {
    return Vector3f(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
}