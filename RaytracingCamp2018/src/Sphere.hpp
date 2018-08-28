#pragma once

#include "IIntersectable.hpp"

class Sphere : public IIntersectable
{
public:
    Sphere();
    Sphere(const Sphere &s);
    Sphere(const double radius, const Vec pos, Material *mat, const  bool visibleFromCamera = true);
    void intersect(const Ray &ray, int depth, Intersection &isect);

    double radius;
    Vec pos;
};

