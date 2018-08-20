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
    return *this * -1;
}

Vec Vec::reflect(const Vec &n) const
{
    return *this - n.scale(2 * this->dot(n));
}

Vec Vec::refract(const Vec &n, double eta) const
{
    auto dot = this->dot(n);
    auto d = 1.0 - std::pow(eta, 2) * (1.0 - std::pow(dot, 2));
    if (d > 0) {
        auto a = (*this - n.scale(dot)).scale(eta);
        auto b = n.scale(std::sqrt(d));
        return a - b;
    }
    // 全反射
    return this->reflect(n);
}

Vec Vec::normalize() const
{
    return this->scale(1 / this->len());
}

Vec Vec::scale(const double s) const
{
    return Vec(x * s, y * s, z * s);
}

Vec Vec::randomHemisphere() const
{
    Random &rand = Random::get_instance();
    Vec v = Vec(1);
    while (v.len() > 1) {
        v.x = rand.random(-1.0, 1.0);
        v.y = rand.random(-1.0, 1.0);
        v.z = rand.random(-1.0, 1.0);
    }
    if (v.dot(*this) < 0) {
        v = v.neg();
    }
    return v.normalize();
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

bool Vec::equals(const Vec &v) const
{
    return x == v.x && y == v.y && z == v.z;
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