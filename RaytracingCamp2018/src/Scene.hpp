#pragma once

#define USE_PATH_TRACING

#include <vector>

#include "IIntersectable.hpp"
#include "Sphere.hpp"
#include "ObjLoader.hpp"
#include "PolygonObject.hpp"
#include "InifinitePlane.hpp"
#include "Light.hpp"
#include "constant.h"
#include "CubeMapImage.hpp"

class Scene
{
public:
    Scene();
    ~Scene();
    void addIntersectable(IIntersectable *obj);
    void addObj(
        const std::string fileName,
        const Vec &pos,
        const Vec &scale,
        const Vec &rot,
        const Material &mat,
        const int cloneNum = 1, const Vec posOffset = Vec(), const Vec scaleOffset = Vec(), const Vec rotOffset = Vec(), bool scaleReflect = false);
    void addLight(Light *light);
    void addCubeMapImage(CubeMapImage *image, int dir);
    void trace(const Ray &ray, Spectrum &spectrum, int depth = 0);
    void intersectSurface(const Vec &dir, const Intersection &isect, Spectrum &spectrum, double eta, int depth);

private:
    std::vector<IIntersectable*> *objs;
    std::vector<Light*> *lights;
    CubeMapImage *cubeMapImages[6]{NULL};

    double random() const;
    void findNearestInterSection(const Ray &ray, int depth, Intersection &isect);
    void diffuseLighting(const Vec &p, const Vec &n, const Light &light, const Spectrum &matDiffuse, Spectrum &spectrum);
    bool visible(const Vec &from, const Vec &to);
    void addSkyColor(const Ray &ray, Spectrum &spectrum);
};
