#pragma once

#include "Intersection.hpp"
#include "Ray.hpp"

class IIntersectable
{
public:
    IIntersectable()
        : mat(new Material())
    {};
    IIntersectable(Material *mat, bool visibleFromCamera = true)
        : mat(mat)
        , bVisibleFromCamera(visibleFromCamera)
    {};
    ~IIntersectable() {
        if (mat) delete mat;
    };
    virtual void intersect(const Ray &ray, int depth, Intersection &isect) = 0;
    bool bVisibleFromCamera = true;
    bool visibleFromCamera(int depth)
    {
        return bVisibleFromCamera || depth != 0;
    };
    Material *mat;
};

