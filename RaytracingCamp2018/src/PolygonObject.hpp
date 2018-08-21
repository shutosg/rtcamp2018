#pragma once

// #define POLYGON_BVH

#include "IIntersectable.hpp"
#include "ObjLoader.hpp"
#include "Bvh.hpp"


struct Triangle
{
    Vec *v[3];    // vertexes
    int vIdx[3];  // vertexIndexes
};


class PolygonObject : public IIntersectable
{
public:
    PolygonObject();
    PolygonObject(const ObjLoader &loader, const Vec &pos = Vec(0), const Vec &scale = Vec(1), const Vec &rot = Vec(0));
    ~PolygonObject();
    void intersect(const Ray &ray, Intersection &isect);

private:
    Vec **vertices;
    int vertNum;
    int *verticesIndexes;
    vector<Triangle> triangles;
    bool isSingleSide;
    Material *mat;

    void getVertexes(int polyIdx, Vec *&v0, Vec *&v1, Vec *&v2);
    bool intersectTryangle(const Ray &ray, const Vec &v0, const Vec &v1, const Vec &v2, Intersection &isect);
};

