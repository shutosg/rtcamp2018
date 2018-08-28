#pragma once

#include <cmath>
#include <algorithm>
#include "Vec.hpp"
#include "constant.h"

class Spectrum
{
public:
    Spectrum() : r(0), g(0), b(0) {};
    Spectrum(const Spectrum &s) : r(s.r), g(s.g), b(s.b) {};
    Spectrum(double r, double g, double b) : r(r), g(g), b(b) {};
    Spectrum(double v) : r(v), g(v), b(v) {};
    double r, g, b;

    inline Spectrum scale(const double s) const { return Spectrum(r * s, g * s, b * s); };
    inline Spectrum operator+(const Spectrum &s) const { return Spectrum(r + s.r, g + s.g, b + s.b); };
    inline Spectrum operator-(const Spectrum &s) const { return Spectrum(r - s.r, g - s.g, b - s.b); };
    inline Spectrum operator*(const Spectrum &s) const { return Spectrum(r * s.r, g * s.g, b * s.b); };
    inline Spectrum operator/(const Spectrum &s) const { return Spectrum(r / s.r, g / s.g, b / s.b); };
    inline void operator = (const Spectrum &s)
    {
        r = s.r;
        g = s.g;
        b = s.b;
    };
    inline void operator+=(const Spectrum &s)
    {
        r += s.r;
        g += s.g;
        b += s.b;
    };
    inline bool equals(const Spectrum &s) const { return r == s.r && g == s.g && b == s.b; };
    inline Vec toColor() const
    {
        int ir = (int)std::min(std::pow(r, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
        int ig = (int)std::min(std::pow(g, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
        int ib = (int)std::min(std::pow(b, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
        return Vec(ir, ig, ib);
    };
    inline std::string toStr() const
    {
        return "Spectrum(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")";
    };

    static const Spectrum Red;
    static const Spectrum Green;
    static const Spectrum Blue;
    static const Spectrum Black;
    static const Spectrum Sky;
    static const double DISPLAY_GAMMA;
};
