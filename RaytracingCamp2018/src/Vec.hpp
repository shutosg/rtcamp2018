#pragma once
#include <cmath>
#include <string>

#include "constant.h"
#include "Random.hpp"

class Vec
{
public:
    Vec();
    Vec(const Vec &v);
    Vec(double x, double y, double z);
    Vec(double v);
    double x, y, z;

    Vec neg() const;
    Vec reflect(const Vec &n) const;
    Vec refract(const Vec &n, double eta) const;
    Vec normalize() const;
    Vec scale(const double s) const;
    Vec rotation(const Vec &rot, bool isDegree = true);
    Vec randomHemisphere() const;
    double len() const;
    Vec operator + (const Vec &v) const;
    Vec operator - (const Vec &v) const;
    Vec operator * (const Vec &v) const;
    Vec operator / (const Vec &v) const;
    bool equals(const Vec &v) const;
    double dot(const Vec &v) const;
    Vec cross(const Vec &v) const;
    std::string toStr() const;
};

