#include "InifinitePlane.hpp"

InfinitePlane::InfinitePlane()
    : IIntersectable()
    , normal(Vec(0, 1, 0))
    , d(0)
{
}

InfinitePlane::InfinitePlane(const Vec &normal, const double d, Material *mat)
    : IIntersectable(mat)
    , normal(normal.normalize())
    , d(d)
{
}

InfinitePlane::InfinitePlane(const InfinitePlane &p)
    : InfinitePlane::InfinitePlane(p.normal, p.d, new Material(*p.mat))
{
}

void InfinitePlane::intersect(const Ray &ray, int depth, Intersection &isect)
{
    auto v = normal.dot(ray.dir);
    auto t = -(normal.dot(ray.origin) + d) / v;
    if (0 < t) {
        isect.t = t;
        isect.point = ray.origin + ray.dir.scale(t);
        isect.normal = normal;
        isect.mat = mat;
    }
}