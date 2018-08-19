#pragma once

#include "Vec.hpp"
#include "Ray.hpp"

class Camera
{
public:
    Camera();
    Camera
    (
        const Vec &pos,
        const Vec &dir,
        const Vec &up,
        double focalDist,
        double aspect
    );
    Ray getPrimaryRay(double x, double y);
    ~Camera();
private:
    Vec eye;
    Vec xAxis;
    Vec yAxis;
    Vec centerDir;
    double aspect;
};

