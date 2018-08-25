#pragma once

#include "IIntersectable.hpp"

class AABBox : public IIntersectable
{
public:
    AABBox();
    AABBox(const AABBox &aabbox);
    AABBox(Vec minPoint, Vec maxPoint);
    Vec minPoint;
    Vec maxPoint;
    void intersect(const Ray &ray, int depth, Intersection &isect);
    Vec getCenter();
    double getSurfaceArea();
};

