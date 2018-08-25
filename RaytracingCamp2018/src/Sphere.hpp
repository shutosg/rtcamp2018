#pragma once

#include "IIntersectable.hpp"

class Sphere : public IIntersectable
{
public:
    Sphere();
    Sphere(const Sphere &sphere);
    Sphere(double radius, Vec *pos, Material *mat, bool visibleFromCamera = true);
    ~Sphere();
    void intersect(const Ray &ray, int depth, Intersection &isect);

    double radius;
    Vec *pos;
};

