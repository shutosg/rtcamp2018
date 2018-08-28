#include "Ray.hpp"

Ray::Ray()
    : origin(Vec(0))
    , dir(Vec(0, 0, 1))
{
}

Ray::Ray(const Vec &origin, const Vec &dir, bool isAdvanceRay)
    : origin(origin)
    , dir(dir.normalize())
{
    if(isAdvanceRay) {
        this->origin = origin + dir.scale(constants::kEPS);
    }
}


Ray::~Ray()
{
}
