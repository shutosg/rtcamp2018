#pragma once

#include "Vec.hpp"
#include "Ray.hpp"
#include "Random.hpp"

class Camera
{
public:
    Camera();
    Camera(const Vec &pos, const Vec &dir, const Vec &up, double imagePlane, double focalDist, double lensRadius, double aspect);
    Ray getPrimaryRay(double x, double y, int width, int height);
    ~Camera();
private:
    Vec eye;
    Vec xAxis;
    Vec yAxis;
    Vec dir;
    double imagePlane;
    double focalDistance;
    double lensRadius;
    double aspect;
};

