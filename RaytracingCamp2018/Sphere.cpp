#include "Sphere.hpp"

Sphere::Sphere()
    : radius(1)
    , pos(new Vec())
    , mat(new Material())
{
}

Sphere::Sphere(const Sphere &sphere)
    : radius(sphere.radius)
    , pos(sphere.pos)
    , mat(sphere.mat)
{
}

Sphere::Sphere(double radius, Vec *pos, Material *mat)
    : radius(radius)
    , mat(mat)
    , pos(pos)
{
}

Sphere::~Sphere()
{
    delete mat;
    delete pos;
}

void Sphere::intersect(const Ray &ray, Intersection &isect)
{
    auto p = ray.origin - *pos;
    auto b = ray.dir.dot(p);
    auto c = p.dot(p) - radius * radius;

    // 交差判定の方程式の判別式
    auto d = b * b -  c;
    // auto d = b * b - 4 * a * c;
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