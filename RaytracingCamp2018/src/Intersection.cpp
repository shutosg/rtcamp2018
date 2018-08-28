#include "Intersection.hpp"

Intersection::Intersection()
    : t(constants::kINF)
    , point(Vec())
    , normal(Vec())
    , mat(NULL)
{
}

void Intersection::operator=(const Intersection &isect)
{
    t = isect.t;
    point = isect.point;
    normal = isect.normal;
    mat = isect.mat;
}

bool Intersection::isHit()
{
    return t != constants::kINF;
}