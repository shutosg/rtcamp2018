#pragma once

#include "IIntersectable.hpp"

class Disc : public IIntersectable
{
public:
    Disc();
    Disc(const Disc &p);
    Disc(const Vec &pos, const Vec &normal, const double radius = 1, const bool doubleSurface = true, Material *mat = new Material());
    void intersect(const Ray &ray, int depth, Intersection &isect);
    void intersectPlane(const Ray &ray, Intersection &isect);

private:
    Vec pos;
    Vec normal;
    double radius;
    bool doubleSurface;
};

