﻿#include "Camera.hpp"



Camera::Camera()
{
}

Camera::Camera(
    const Vec & pos,
    const Vec & dir,
    const Vec & up,
    double focalDist,
    double aspect)
{
    eye = pos;
    xAxis = dir.cross(up).normalize();
    yAxis = up.normalize();
    centerDir = dir.normalize().scale(focalDist);
    this->aspect = aspect;
}

Ray Camera::getPrimaryRay(double x, double y)
{
    auto dir = centerDir + xAxis * x * aspect + yAxis * y;
    return Ray(eye, dir.normalize());
}

Camera::~Camera()
{
}