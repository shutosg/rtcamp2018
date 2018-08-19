#pragma once

#include "IIntersectable.hpp"

class Sphere : public IIntersectable
{
public:
    Sphere();
    Sphere(const Sphere &sphere);
    Sphere(double radius, Vec *pos, Material *mat);
    ~Sphere();
    void intersect(const Ray &ray, Intersection &isect);

    double radius;
    Vec *pos;
    Material *mat;
};

