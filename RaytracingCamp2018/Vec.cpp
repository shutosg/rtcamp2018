#include "Vec.hpp"

Vec::Vec()
{
    x = y = z = 0;
}

Vec::Vec(const Vec &v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
{
}

Vec::Vec(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{
}

Vec::Vec(double v)
    : x(v)
    , y(v)
    , z(v)
{
}

Vec Vec::neg() const
{
    return Vec(-x, -y, -z);
}

Vec Vec::reflect(const Vec &n) const
{
    return Vec(x, y, z) - n.scale(2 * dot(n));
}

Vec Vec::refract(const Vec &n, double eta) const
{
    auto dot = Vec::dot(n);
    auto d = 1.0 - eta * eta * (1.0 - dot * dot);
    if (d > 0) {
        Vec a = (operator-(n.scale(dot))).scale(eta);
        Vec b = n.scale(d * d);
        return a - b;
    }
    // 全反射
    return reflect(n);
}

Vec Vec::normalize() const
{
    return this->scale(1 / this->len());
}

Vec Vec::scale(const double s) const
{
    return Vec(x * s, y * s, z * s);
}

double Vec::len() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vec Vec::operator+(const Vec &v) const
{
    return Vec(x + v.x, y + v.y, z + v.z);
}

Vec Vec::operator-(const Vec &v) const
{
    return Vec(x - v.x, y - v.y, z - v.z);
}

Vec Vec::operator/(const Vec &v) const
{
    return Vec(x / v.x, y / v.y, z / v.z);
}

Vec Vec::operator*(const Vec &v) const
{
    return Vec(x * v.x, y * v.y, z * v.z);
}

double Vec::dot(const Vec &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vec Vec::cross(const Vec &v) const
{
    return Vec(
        y * v.z - v.y * z,
        z * v.x - v.z * x,
        x * v.y - v.x * y
    );
}

std::string Vec::toStr() const
{
    return "Vec(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}