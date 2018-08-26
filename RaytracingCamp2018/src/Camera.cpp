#include "Camera.hpp"



Camera::Camera()
{
}

Camera::Camera(const Vec & pos, const Vec & dir, const Vec & up, double imagePlane, double focalDist, double lensRadius, double aspect)
    : eye(pos)
    , xAxis(dir.cross(up).normalize())
    , yAxis(up.normalize())
    , dir(dir.normalize())
    , imagePlane(dir.normalize().scale(imagePlane).len())
    , focalDistance(focalDist)
    , lensRadius(lensRadius)
    , aspect(aspect)
{
}

Ray Camera::getPrimaryRay(double x, double y, int width, int height)
{
    // アンチエイリアシング
    Random &rnd = Random::get_instance();
    x += rnd.random(-0.5, 0.5) / width;
    y += rnd.random(-0.5, 0.5) / height;

    auto rayDir = dir.scale(imagePlane) + xAxis * x * aspect + yAxis * y;
    auto eyeOffset = Vec(lensRadius, lensRadius, 0);

    // lensRadiusが0より大きければ被写界深度を考慮
    if (lensRadius > 0) {
        Random &rnd = Random::get_instance();
        while (eyeOffset.len() > lensRadius) {
            eyeOffset.x = rnd.random(-lensRadius, lensRadius);
            eyeOffset.y = rnd.random(-lensRadius, lensRadius);
        }
    }
    return Ray(eye + eyeOffset, (rayDir.normalize().scale(focalDistance) - eyeOffset).normalize());
}

Camera::~Camera()
{
}
