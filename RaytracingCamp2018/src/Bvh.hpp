#pragma once
#include <cmath>
#include <string>
#include <algorithm>

#include "Vec.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"
#include "AABBox.hpp"

class Bvh
{
public:
    Bvh();
    Bvh(const std::vector<Triangle> &tris, std::vector<Bvh*> &nodeList);
    ~Bvh();
    Bvh(const Bvh &bvh);
    AABBox *aabbox;
    Bvh *childs[2];
    std::vector<Triangle> tris;
    std::vector<Triangle> findCandidates(const Ray &ray) const;
    std::string toStr() const;
};
