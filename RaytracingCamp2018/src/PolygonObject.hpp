#pragma once
#include "IIntersectable.hpp"
class PolygonObject : public IIntersectable
{
public:
    PolygonObject();
    ~PolygonObject();
    void intersect(const Ray &ray, Intersection &isect);
    static void  loadObject(const std::string &fileName, std::vector<double> *vertices);

private:
    Vec **vertices;
    int vertNum;
    int polyNum;
    int *faceIndexes;
    int *verticesIndexes;
    bool isSingleSide;
    Material *mat;

    bool intersectTryangle(const Ray &ray, const Vec &v0, const Vec &v1, const Vec &v2, Intersection &isect);
};

