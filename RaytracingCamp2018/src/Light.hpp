#pragma once

#include "Vec.hpp"
#include "Spectrum.hpp"

class Light
{
public:
    Light() : pos(Vec(1.0)), power(Spectrum(10)) {};
    Light(const Vec &pos, const Spectrum &power) : pos(pos), power(power) {};
    Light(const Light &l) : Light::Light(l.pos, l.power) {};

    Vec pos;
    Spectrum power;
};

