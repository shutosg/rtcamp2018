#include "Disc.hpp"

Disc::Disc()
    : IIntersectable()
    , pos(Vec(0))
    , normal(Vec(0, 0, 1))
    , doubleSurface(false)
{
}

Disc::Disc(const Vec &pos, const Vec &normal, const double radius, const bool doubleSurface, Material *mat)
    : IIntersectable(mat)
    , pos(pos)
    , normal(normal.normalize())
    , radius(radius)
    , doubleSurface(doubleSurface)
{
}

Disc::Disc(const Disc &d)
    : Disc::Disc(d.pos, d.normal, d.radius, d.doubleSurface, new Material(*d.mat))
{
}

void Disc::intersect(const Ray &ray, int depth, Intersection &isect)
{
    if (!visibleFromCamera(depth)) return;
    intersectPlane(ray, isect);
    if (isect.isHit()) {
        auto v = isect.point - pos;
        if (std::sqrt(v.dot(v)) <= radius) {
            // ヒットしたのでマテリアル情報を渡す
            *isect.mat = *mat;
        }
        else {
            // 半径の外なのでisectを戻す
            isect.t = constants::kINF;
            isect.point = Vec();
            isect.normal = Vec();
        }
    }
}

void Disc::intersectPlane(const Ray &ray, Intersection &isect)
{
    double normalDotDir = normal.dot(ray.dir);
    // 裏を向いてるので交差しない
    if (!doubleSurface && normalDotDir < 0) return;
    // rayが面と垂直なので交差しない
    if (std::abs(normalDotDir) <= constants::kEPS) return;

    // 交差する
    auto v = pos - ray.origin;
    auto t = v.dot(normal) / normalDotDir;
    if (t >= 0) {
        isect.t = t;
        isect.point = ray.origin + ray.dir.scale(t);
        isect.normal = normal;
    }
}