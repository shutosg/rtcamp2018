#include "Sphere.hpp"

Sphere::Sphere()
    : IIntersectable()
    , radius(1)
    , pos(Vec())
{
}

Sphere::Sphere(const double radius, const Vec pos, Material *mat, const bool visibleFromCamera)
    : IIntersectable(mat, visibleFromCamera)
    , radius(radius)
    , pos(pos)
{
}

Sphere::Sphere(const Sphere &s)
    : Sphere::Sphere(s.radius, s.pos, new Material(*s.mat), s.bVisibleFromCamera)
{
}

void Sphere::intersect(const Ray &ray, int depth, Intersection &isect)
{
    if (!visibleFromCamera(depth)) return;
    auto p = ray.origin - pos;
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
            isect.point = ray.origin + ray.dir.scale(t);
            isect.normal = (isect.point - pos).normalize();
            isect.mat = mat;
        }
    }
}