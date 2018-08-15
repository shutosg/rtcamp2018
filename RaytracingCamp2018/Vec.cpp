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
{
    this->x = v;
    this->y = v;
    this->z = v;
}

Vec Vec::neg() const
{
    return Vec(-x, -y, -z);
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