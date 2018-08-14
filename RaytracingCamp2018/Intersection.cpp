#include "Intersection.hpp"

Intersection::Intersection()
{
    t = constants::kINF;
    point = new Vec();
    normal = new Vec();
    mat = new Material();
}

Intersection::~Intersection()
{
    delete point;
    delete normal;
    delete mat;
}

void Intersection::operator=(const Intersection &isect)
{
    t = isect.t;
    *point = *(isect.point);
    *normal = *(isect.normal);
    *mat = *(isect.mat);
}

bool Intersection::isHit()
{
    return t != constants::kINF;
}