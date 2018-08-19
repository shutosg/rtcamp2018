#pragma once

#include "Intersection.hpp"
#include "Ray.hpp"

class IIntersectable
{
public:
    IIntersectable() {};
    ~IIntersectable() {};
    virtual void intersect(const Ray &ray, Intersection &isect) = 0;
};

