#pragma once

#include "Vec.hpp"
#include "constant.h"

class Ray
{
public:
    Ray();
    Ray(const Vec &origin, const Vec &dir, bool isAdvanceRay = false);
    ~Ray();
    Vec origin;
    Vec dir;
};
