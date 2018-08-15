#pragma once

#include "Spectrum.hpp"

class Material
{
public:
    Material();
    Material(const Material &m);
    Material(Spectrum *diffuse, double reflective = 0, double refractive = 0, double refractiveIndex = 1);
    ~Material();
    void operator=(const Material &m);

    Spectrum *diffuse;
    double reflective;
    double refractive;
    double refractiveIndex;
};

