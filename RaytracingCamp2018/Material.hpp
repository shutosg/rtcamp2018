#pragma once

#include "Spectrum.hpp"

class Material
{
public:
    Material();
    Material(const Material &material);
    Material(Spectrum *diffuse);
    ~Material();
    void operator=(const Material &m);
    Spectrum *diffuse;
};

