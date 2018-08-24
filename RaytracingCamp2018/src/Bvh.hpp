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
    Bvh(std::vector<Triangle> tris);
    ~Bvh();
    Bvh(const Bvh &bvh);
    AABBox *aabbox;
    Bvh *childs[2];
    std::vector<Triangle> tris;
    std::vector<Triangle> findCandidates(const Ray &ray) const;
    std::string toStr() const;
    const double CostAABBIntersection = 1.0;
    const double CostTriangleIntersection = 1.0;
private:
    void sortTriangle(std::vector<Triangle> &tris, int axis);
    void findBestSplit(std::vector<Triangle> tris, int &bestAxis, int &bestIndex);
};
