#include "Spectrum.hpp"

Spectrum::Spectrum()
{
    r = g = b = 0;
}

Spectrum::Spectrum(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Spectrum::Spectrum(double v)
{
    this->r = v;
    this->g = v;
    this->b = v;
}

Spectrum Spectrum::scale(const double s)
{
    return Spectrum(this->r * s, this->g * s, this->b * s);
}


Spectrum Spectrum::operator+(const Spectrum &s)
{
    return Spectrum(this->r + s.r, this->g + s.g, this->b + s.b);
}

Spectrum Spectrum::operator-(const Spectrum &s)
{
    return Spectrum(this->r - s.r, this->g - s.g, this->b - s.b);
}

Spectrum Spectrum::operator/(const Spectrum &s)
{
    return Spectrum(this->r / s.r, this->g / s.g, this->b / s.b);
}

Spectrum Spectrum::operator*(const Spectrum &s)
{
    return Spectrum(this->r * s.r, this->g * s.g, this->b * s.b);
}

void Spectrum::operator=(const Spectrum &s)
{
    this->r = s.r;
    this->g = s.g;
    this->b * s.b;
}

Vec Spectrum::toColor()
{
    int ir = (int)std::min(std::pow(this->r, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
    int ig = (int)std::min(std::pow(this->g, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
    int ib = (int)std::min(std::pow(this->b, 1.0 / DISPLAY_GAMMA) * 255, 255.0);
    return Vec(ir, ig, ib);
}

std::string Spectrum::toStr()
{
    return "Spectrum(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")";
}