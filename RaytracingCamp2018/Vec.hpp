#pragma once
#include <cmath>
#include <string>

class Vec
{
public:
    Vec();
    Vec(double x, double y, double z);
    Vec(double v);
    double x, y, z;

    Vec neg() const;
    Vec normalize() const;
    Vec scale(const double s) const;
    double len() const;
    Vec operator + (const Vec &v) const;
    Vec operator - (const Vec &v) const;
    Vec operator * (const Vec &v) const;
    Vec operator / (const Vec &v) const;
    double dot(const Vec &v) const;
    Vec cross(const Vec &v) const;
    std::string toStr() const;
};

