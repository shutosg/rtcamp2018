#pragma once

#include "Ray.hpp"
#include "Material.hpp"
#include "constant.h"

class Intersection
{
public:
    Intersection();
    ~Intersection();
    void operator=(const Intersection &isect);
    bool isHit();

    double t;
    Vec *point;
    Vec *normal;
    Material *mat;
};

