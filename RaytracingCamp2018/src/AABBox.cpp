#include "AABBox.hpp"

AABBox::AABBox()
    : minPoint(Vec(+constants::kINF))
    , maxPoint(Vec(-constants::kINF))
{
}

AABBox::AABBox(const AABBox &aabbox)
    : minPoint(aabbox.minPoint)
    , maxPoint(aabbox.maxPoint)
{
}

AABBox::AABBox(Vec minPoint, Vec maxPoint)
    : minPoint(minPoint)
    , maxPoint(maxPoint)
{
}

void AABBox::intersect(const Ray &ray, Intersection &isect)
{
    // 参考: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

    // xy平面でチェック
    // yz境界面との交点
    auto txmin = (minPoint.x - ray.origin.x) / ray.dir.x;
    auto txmax = (maxPoint.x - ray.origin.x) / ray.dir.x;
    if (txmin > txmax) std::swap(txmin, txmax);

    // xz境界面との交点
    auto tymin = (minPoint.y - ray.origin.y) / ray.dir.y;
    auto tymax = (maxPoint.y - ray.origin.y) / ray.dir.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    // aabboxの外を通り過ぎていく場合
    if ((txmin > tymax) || (tymin > txmax)) return;

    // xy境界面との交点
    auto tzmin = (minPoint.z - ray.origin.z) / ray.dir.z;
    auto tzmax = (maxPoint.z - ray.origin.z) / ray.dir.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    // 現時点での交点候補を表すt
    auto tmin = std::max(txmin, tymin);
    auto tmax = std::min(txmax, tymax);
    if ((tmin > tzmax) || (tzmin > tmax)) return;

    // ray.dirを含む直線がAABBと交差するので、tを求める
    tmin = std::max(tmin, tzmin);
    tmax = std::min(tmax, tzmax);

    // tmin、tmaxのうち、0より大きく、かつ小さい方がt
    if (tmin < 0) tmin = tmax;
    if (tmin > 0) isect.t = tmin;
}

Vec AABBox::getCenter()
{
    return minPoint.average(maxPoint);
}

// 表面積の算出
double AABBox::getSurfaceArea()
{
    auto w = maxPoint.x - minPoint.x;
    auto h = maxPoint.y - minPoint.y;
    auto d = maxPoint.z - minPoint.z;
    return 2.0 * (w * h + h * d + w * d);
}