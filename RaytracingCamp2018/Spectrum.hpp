#pragma once

#include <cmath>
#include <algorithm>
#include "Vec.hpp"
#include "constant.h"

class Spectrum
{
public:
    Spectrum();
    Spectrum(double r, double g, double b);
    Spectrum(double v);
    double r, g, b;

    Spectrum scale(const double s);
    Spectrum operator + (const Spectrum &s);
    Spectrum operator - (const Spectrum &s);
    Spectrum operator * (const Spectrum &s);
    Spectrum operator / (const Spectrum &s);
    void operator = (const Spectrum &s);
    Vec toColor();
    std::string toStr();
private:
    const double DISPLAY_GAMMA = 2.2;
};
