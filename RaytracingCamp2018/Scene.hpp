#pragma once

#include <vector>
#include "IIntersectable.hpp"
#include "Sphere.hpp"

class Scene
{
public:
    Scene();
    ~Scene();
    void addIntersectable(IIntersectable *obj);
    void trace(const Ray &ray, Spectrum &spectrum);

private:
    std::vector<IIntersectable*> *objs;

    void findNearestInterSection(const Ray &ray, Intersection &isect);
};

