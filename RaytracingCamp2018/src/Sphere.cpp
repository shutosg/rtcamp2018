#include "Sphere.hpp"

Sphere::Sphere()
    : IIntersectable()
    , radius(1)
    , pos(new Vec())
{
}

Sphere::Sphere(const Sphere &sphere)
    : IIntersectable(sphere.mat, sphere.bVisibleFromCamera)
    , radius(sphere.radius)
    , pos(sphere.pos)
{
}

Sphere::Sphere(double radius, Vec *pos, Material *mat, bool visibleFromCamera)
    : IIntersectable(mat, visibleFromCamera)
    , radius(radius)
    , pos(pos)
{
}

Sphere::~Sphere()
{
    delete pos;
}

void Sphere::intersect(const Ray &ray, int depth, Intersection &isect)
{
    if (!visibleFromCamera(depth)) return;
    auto p = ray.origin - *pos;
    auto b = ray.dir.dot(p);
    auto c = p.dot(p) - radius * radius;

    // 交差判定の方程式の判別式
    auto d = b * b -  c;

    if (d >= 0) {
        // 交点までの距離を計算
        auto s = std::sqrt(d);
        auto t = -b - s;
        if (t <= 0) t = -b + s;
        if (t > 0) {
            isect.t = t;
            *isect.point = ray.origin + ray.dir.scale(t);
            *isect.normal = (*isect.point - *pos).normalize();
            *isect.mat = *mat;
        }
    }
}