#include "Plane.hpp"

Plane::Plane()
    : IIntersectable()
    , normal(new Vec(0, 1, 0))
    , d(0)
{
}

Plane::Plane(Vec *normal, double d, Material *mat)
    : IIntersectable(mat)
    , normal(normal)
    , d(d)
{
}

Plane::Plane(const Plane &p)
    : IIntersectable(new Material(*p.mat))
    , normal(new Vec(*p.normal))
    , d(p.d)
{
}

Plane::~Plane()
{
    delete normal;
}

void Plane::intersect(const Ray &ray, int depth, Intersection &isect)
{
    auto v = normal->dot(ray.dir);
    auto t = -(normal->dot(ray.origin) + d) / v;
    if (0 < t) {
        isect.t = t;
        *isect.point = ray.origin + ray.dir.scale(t);
        *isect.normal = *normal;
        *isect.mat = *mat;
    }
}