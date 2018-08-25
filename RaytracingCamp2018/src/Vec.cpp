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

Vec Vec::rotation(const Vec &rot, bool isDegree)
{
    auto thete = isDegree ? Vec(rot) * constants::kPI / 180.0 : Vec(rot);
    auto sinX = std::sin(thete.x);
    auto sinY = std::sin(thete.y);
    auto sinZ = std::sin(thete.z);
    auto cosX = std::cos(thete.x);
    auto cosY = std::cos(thete.y);
    auto cosZ = std::cos(thete.z);
    auto newX = (cosX * cosY * cosZ - sinX * sinZ) * x - (cosX * cosY * sinZ + sinX * cosZ) * y + cosX * sinY * z;
    auto newY = (sinX * cosY * cosZ + cosX * sinZ) * x - (sinX * cosY * sinZ - cosX * cosZ) * y + sinX * sinY * z;
    auto newZ =                      - sinY * cosZ * x                        + sinY * sinZ * y        + cosY * z;
    return Vec(newX, newY, newZ);
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

Vec Vec::min(const Vec &v) const
{
    return Vec(
        std::min(x, v.x),
        std::min(y, v.y),
        std::min(z, v.z)
    );
}

Vec Vec::max(const Vec &v) const
{
    return Vec(
        std::max(x, v.x),
        std::max(y, v.y),
        std::max(z, v.z)
    );
}

Vec Vec::average(const Vec &v) const
{
    return Vec(
        (x + v.x) / 2.0,
        (y + v.y) / 2.0,
        (z + v.z) / 2.0
    );
}

Vec Vec::flag() const
{
    return Vec(
        x == 0 ? 0 : 1,
        y == 0 ? 0 : 1,
        z == 0 ? 0 : 1
    );
}

std::string Vec::toStr() const
{
    return "Vec(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}