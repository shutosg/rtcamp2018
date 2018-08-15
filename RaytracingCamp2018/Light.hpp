#pragma once

#include "Vec.hpp"
#include "Spectrum.hpp"

class Light
{
public:
    Light();
    Light(Vec *pos, Spectrum *power);
    Light(const Light &l);
    ~Light();

    Vec *pos;
    Spectrum *power;
};

