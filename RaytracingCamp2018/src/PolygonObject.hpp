#pragma once

#define POLYGON_BVH

#include "IIntersectable.hpp"
#include "ObjLoader.hpp"
#include "Bvh.hpp"
#include "Triangle.hpp"


class PolygonObject : public IIntersectable
{
public:
    PolygonObject();
    PolygonObject(const ObjLoader &loader, const Vec &pos = Vec(0), const Vec &scale = Vec(1), const Vec &rot = Vec(0), Material *mat = new Material());
    ~PolygonObject();
    void intersect(const Ray &ray, int depth, Intersection &isect);

private:
    Vec **vertices;
    int vertNum;
    int *verticesIndexes;
    vector<Triangle> triangles;
    bool isSingleSide;
#ifdef POLYGON_BVH
    vector<Bvh*> *nodeList;
    void constructBVH();
#endif

    void getVertexes(int polyIdx, Vec *&v0, Vec *&v1, Vec *&v2);
    bool intersectTryangle(const Ray &ray, const Vec &v0, const Vec &v1, const Vec &v2, Intersection &isect);
};
