#pragma once

#include "IIntersectable.hpp"

class Plane : public IIntersectable
{
public:
    Plane();
    Plane(const Plane &p);
    Plane(Vec *normal, double d, Material *mat);
    ~Plane();
    void intersect(const Ray &ray, int depth, Intersection &isect);

private:
    Vec *normal;
    double d;
};

