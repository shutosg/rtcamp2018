#pragma once

#include <vector>
#include "IIntersectable.hpp"
#include "Sphere.hpp"
#include "Light.hpp"
#include "constant.h"

class Scene
{
public:
    Scene();
    ~Scene();
    void addIntersectable(IIntersectable *obj);
    void addLight(Light *light);
    void trace(const Ray &ray, Spectrum &spectrum);

private:
    std::vector<IIntersectable*> *objs;
    std::vector<Light*> *lights;

    void findNearestInterSection(const Ray &ray, Intersection &isect);
    void diffuseLighting(const Vec &p, const Vec &n, const Light &light, const Spectrum &matDiffuse, Spectrum &spectrum);
};

