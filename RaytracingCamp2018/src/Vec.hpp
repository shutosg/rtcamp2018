#pragma once
#include <cmath>
#include <string>

#include "constant.h"
#include "Random.hpp"

class Vec
{
public:
    Vec() : x(0), y(0), z(0) {};
    Vec(const Vec &v) : x(v.x), y(v.y), z(v.z) {};
    Vec(double x, double y, double z) : x(x), y(y), z(z) {};
    Vec(double v) : x(v), y(v), z(v) {};
    double x, y, z;

    inline double len() const { return std::sqrt(x * x + y * y + z * z); };
    inline Vec scale(const double s) const { return Vec(x * s, y * s, z * s); };
    inline Vec normalize() const { return this->scale(1 / this->len()); };
    inline Vec neg() const { return *this * -1; };
    inline Vec reflect(const Vec &n) const { return *this - n.scale(2 * this->dot(n)); };
    inline Vec refract(const Vec &n, double eta) const
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
    };
    inline Vec rotation(const Vec &rot, bool isDegree = true)
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
        auto newZ = -sinY * cosZ * x + sinY * sinZ * y + cosY * z;
        return Vec(newX, newY, newZ);
    };
    inline Vec randomHemisphere() const
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
    };
    inline Vec operator + (const Vec &v) const { return Vec(x + v.x, y + v.y, z + v.z); };
    inline Vec operator - (const Vec &v) const { return Vec(x - v.x, y - v.y, z - v.z); };
    inline Vec operator * (const Vec &v) const { return Vec(x * v.x, y * v.y, z * v.z); };
    inline Vec operator / (const Vec &v) const { return Vec(x / v.x, y / v.y, z / v.z); };
    inline bool equals(const Vec &v) const { return x == v.x && y == v.y && z == v.z; };
    inline double dot(const Vec &v) const { return x * v.x + y * v.y + z * v.z; };
    inline Vec cross(const Vec &v) const
    {
        return Vec(
            y * v.z - v.y * z,
            z * v.x - v.z * x,
            x * v.y - v.x * y
        );
    };
    inline Vec min(const Vec &v) const
    {
        return Vec(
            std::min(x, v.x),
            std::min(y, v.y),
            std::min(z, v.z)
        );
    };
    inline Vec max(const Vec &v) const
    {
        return Vec(
            std::max(x, v.x),
            std::max(y, v.y),
            std::max(z, v.z)
        );
    };
    inline Vec average(const Vec &v) const
    {
        return Vec(
            (x + v.x) / 2.0,
            (y + v.y) / 2.0,
            (z + v.z) / 2.0
        );
    };
    inline Vec abs() const
    {
        return Vec(
            std::abs(x),
            std::abs(y),
            std::abs(z)
        );
    };
    inline Vec flag() const
    {
        return Vec(
            x == 0 ? 0 : 1,
            y == 0 ? 0 : 1,
            z == 0 ? 0 : 1
        );
    };
    inline int minAxis() const
    {
        auto min = std::min(std::min(x, y), z);
        if (min == x) return 0;
        if (min == y) return 1;
        return 2;
    };
    inline int maxAxis() const
    {
        auto max = std::max(std::max(x, y), z);
        if (max == x) return 0;
        if (max == y) return 1;
        return 2;
    };
    inline std::string toStr() const
    {
        return "Vec(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    };
};

