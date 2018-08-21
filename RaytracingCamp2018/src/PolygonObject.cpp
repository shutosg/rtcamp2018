#include "PolygonObject.hpp"

#include <fstream>
#include <iostream>
#include <vector>

PolygonObject::PolygonObject()
{
    vertNum = 3;
    vertices = new Vec*[vertNum];
    vertices[0] = new Vec(0.5, 0.5, 0.1);
    vertices[1] = new Vec(0, 0.5, 0.2);
    vertices[2] = new Vec(0, 0, 0.1);
    polyNum = 1;
    faceIndexes = new int[1];
    faceIndexes[0] = 3;
    verticesIndexes = new int[3];
    verticesIndexes[0] = 0;
    verticesIndexes[1] = 1;
    verticesIndexes[2] = 2;
    mat = new Material(new Spectrum(1, 0 ,0));
    isSingleSide = true;
}

PolygonObject::PolygonObject(const ObjLoader &loader)
{
    // objloaderからデータをコピー
    // 頂点情報
    vertNum = loader.vertices->size();
    vertices = new Vec*[vertNum];
    for (auto i = 0; i < vertNum; i++) {
        vertices[i] = new Vec(*loader.vertices->at(i));
    }
    // 面情報
    polyNum = loader.faceIndexes->size();
    faceIndexes = new int[polyNum];
    faceIndexes = loader.faceIndexes->data();
    verticesIndexes = new int[loader.verticesIndexes->size()];
    verticesIndexes = loader.verticesIndexes->data();

    // その他
    mat = new Material(new Spectrum(1, 0, 0));
    isSingleSide = mat->refractive == 0;
}

PolygonObject::~PolygonObject()
{
    for (auto i = 0; i < vertNum; i++) {
        delete vertices[i];
    }
    delete vertices;
    delete faceIndexes;
    delete verticesIndexes;
    delete mat;
}

void PolygonObject::intersect(const Ray &ray, Intersection &isect)
{
    // 総当たりで最も近くで交差した点を求める
    Intersection nearest;
    for (auto polyIdx = 0; polyIdx < polyNum; polyIdx++) {
        Intersection tmp;
        Vec *v0, *v1, *v2;
        getVertexes(polyIdx, v0, v1, v2);
        if (intersectTryangle(ray, *v0, *v1, *v2, tmp) && tmp.t < nearest.t) {
            nearest = tmp;
        }
    }
    isect.t = nearest.t;
    *isect.mat = *mat;
    *isect.normal = *nearest.normal;
    *isect.point = *nearest.point;
}

void PolygonObject::getVertexes(int polyIdx, Vec *&v0, Vec *&v1, Vec *&v2)
{
    // NOTE: 頂点idは1始まり
    v0 = vertices[verticesIndexes[polyIdx * 3 + 0] - 1];
    v1 = vertices[verticesIndexes[polyIdx * 3 + 1] - 1];
    v2 = vertices[verticesIndexes[polyIdx * 3 + 2] - 1];
}

bool PolygonObject::intersectTryangle(const Ray &ray, const Vec &v0, const Vec &v1, const Vec &v2, Intersection &isect)
{
    // 参考: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
 
    // 法線を算出
    auto v01 = v1 - v0;
    auto v02 = v2 - v0;
    auto normal = v01.cross(v02).normalize();

    // 面がrayと同じ方向を向いてたら交差しない
    if (isSingleSide && normal.normalize().dot(ray.dir) > 0) return false;

    // 面がrayと平行なら交差しない
    auto nDotRayDir = normal.dot(ray.dir);
    if (std::abs(nDotRayDir) < constants::kEPS) return false;

    // dを算出
    auto d = normal.dot(v0);

    // tを算出
    auto t = (d - normal.dot(ray.origin)) / nDotRayDir;
    // rayの始点より後ろにいるなら交差しない
    if (t < 0) return false;

    // 三角形を含む面との交点を算出
    auto point = ray.origin + ray.dir.scale(t);

    // 交点が三角形の内側にあるか調べる
    Vec C;  // 使い回す
    Vec vp;

    // edge v01
    vp = point - v0;
    C = v01.cross(vp);
    if (normal.dot(C) < 0) return false;
    // edge v12
    Vec v12 = v2 - v1;
    vp = point - v1;
    C = v12.cross(vp);
    if (normal.dot(C) < 0) return false;
    // edge v20
    Vec v20 = v0 - v2;
    vp = point - v2;
    C = v20.cross(vp);
    if (normal.dot(C) < 0) return false;

    // ヒットするのでtを入れて返す
    isect.t = t;
    *isect.normal = normal;
    *isect.point = point;
    return true;
}