#pragma once

#include "IIntersectable.hpp"
#include "ObjLoader.hpp"

class PolygonObject : public IIntersectable
{
public:
    PolygonObject();
    PolygonObject(const ObjLoader &loader);
    ~PolygonObject();
    void intersect(const Ray &ray, Intersection &isect);

private:
    Vec **vertices;
    int vertNum;
    int polyNum;
    int *faceIndexes;
    int *verticesIndexes;
    bool isSingleSide;
    Material *mat;

    void getVertexes(int polyIdx, Vec *&v0, Vec *&v1, Vec *&v2);
    bool intersectTryangle(const Ray &ray, const Vec &v0, const Vec &v1, const Vec &v2, Intersection &isect);
};

