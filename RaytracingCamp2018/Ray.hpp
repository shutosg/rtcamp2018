#pragma once

#include "Vec.hpp"
#include "constant.h"

class Ray
{
public:
    Ray();
    Ray(const Vec &origin, const Vec &dir);
    Ray(const Vec &origin, const Vec &dir, bool isAdvanceRay);
    ~Ray();
    Vec origin;
    Vec dir;
};
