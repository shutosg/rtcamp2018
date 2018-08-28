#pragma once

#include "IIntersectable.hpp"

class InfinitePlane : public IIntersectable
{
public:
    InfinitePlane();
    InfinitePlane(const InfinitePlane &p);
    InfinitePlane(const Vec &normal, const double d, Material *mat);
    void intersect(const Ray &ray, int depth, Intersection &isect);

private:
    Vec normal;
    double d;
};

