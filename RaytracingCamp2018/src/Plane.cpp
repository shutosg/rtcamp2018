#include "Plane.hpp"

Plane::Plane()
    : normal(new Vec(0, 1, 0))
    , d(0)
    , mat(new Material())
{
}

Plane::Plane(Vec *normal, double d, Material *mat)
    : normal(normal)
    , d(d)
    , mat(mat)
{
}

Plane::Plane(const Plane &p)
    : normal(new Vec(*p.normal))
    , d(p.d)
    , mat(new Material(*p.mat))
{
}

Plane::~Plane()
{
    delete normal;
    delete mat;
}

void Plane::intersect(const Ray &ray, Intersection &isect)
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