#pragma once

#include "IIntersectable.hpp"

class InfinitePlane : public IIntersectable
{
public:
    InfinitePlane();
    InfinitePlane(const InfinitePlane &p);
    InfinitePlane(Vec *normal, double d, Material *mat);
    ~InfinitePlane();
    void intersect(const Ray &ray, int depth, Intersection &isect);

private:
    Vec *normal;
    double d;
};

