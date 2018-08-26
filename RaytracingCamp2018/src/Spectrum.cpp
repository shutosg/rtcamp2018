#include "Spectrum.hpp"

Spectrum::Spectrum()
{
    r = g = b = 0;
}

Spectrum::Spectrum(const Spectrum &s)
    : r(s.r)
    , g(s.g)
    , b(s.b)
{
}

Spectrum::Spectrum(double r, double g, double b)
    : r(r)
    , g(g)
    , b(b)
{
}

Spectrum::Spectrum(double v)
    : r(v)
    , g(v)
    , b(v)
{
}

Spectrum Spectrum::scale(const double s)
{
    return Spectrum(r * s, g * s, b * s);
}


Spectrum Spectrum::operator+(const Spectrum &s)
{
    return Spectrum(r + s.r, g + s.g, b + s.b);
}

Spectrum Spectrum::operator-(const Spectrum &s)
{
    return Spectrum(r - s.r, g - s.g, b - s.b);
}

Spectrum Spectrum::operator/(const Spectrum &s)
{
    return Spectrum(r / s.r, g / s.g, b / s.b);
}

Spectrum Spectrum::operator*(const Spectrum &s)
{
    return Spectrum(r * s.r, g * s.g, b * s.b);
}

void Spectrum::operator=(const Spectrum &s)
{
    r = s.r;
    g = s.g;
    b = s.b;
}

void Spectrum::operator+=(const Spectrum &s)
{
    r += s.r;
    g += s.g;
    b += s.b;
}

bool Spectrum::equals(const Spectrum &s)
{
    return r == s.r && g == s.g && b == s.b;
}

Vec Spectrum::toColor()
{
    int ir = (int)std::min(std::pow(r, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
    int ig = (int)std::min(std::pow(g, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
    int ib = (int)std::min(std::pow(b, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
    return Vec(ir, ig, ib);
}

std::string Spectrum::toStr()
{
    return "Spectrum(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")";
}

const Spectrum Spectrum::Black = Spectrum(0);
const Spectrum Spectrum::Sky = Spectrum(0.8, 0.8, 0.8);