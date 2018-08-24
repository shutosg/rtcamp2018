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
    verticesIndexes = new int[3];
    verticesIndexes[0] = 0;
    verticesIndexes[1] = 1;
    verticesIndexes[2] = 2;
    mat = new Material(new Spectrum(1, 0 ,0));
    isSingleSide = true;
}

PolygonObject::PolygonObject(const ObjLoader &loader, const Vec &pos, const Vec &scale, const Vec &rot, Material *mat)
{
    // objloaderからデータをコピー
    // 頂点情報
    vertNum = loader.vertices->size();
    vertices = new Vec*[vertNum];
    for (auto i = 0; i < vertNum; i++) {
        vertices[i] = new Vec(*loader.vertices->at(i));
    }
    // 面情報
    verticesIndexes = new int[loader.verticesIndexes->size()];
    verticesIndexes = loader.verticesIndexes->data();
    int polyNum = loader.faceIndexes->size();
    for (auto i = 0; i < polyNum * 3; i += 3) {
        Triangle t;
        t.vIdx[0] = verticesIndexes[i + 0];
        t.vIdx[1] = verticesIndexes[i + 1];
        t.vIdx[2] = verticesIndexes[i + 2];
        // NOTE: verticesIndexは1始まりなので配列のアクセスには1引いた値を使う
        t.v[0] = vertices[t.vIdx[0] - 1];
        t.v[1] = vertices[t.vIdx[1] - 1];
        t.v[2] = vertices[t.vIdx[2] - 1];
        // 重心
        t.center = Vec((*t.v[0] + *t.v[1] + *t.v[2]) / 3.0);
        triangles.push_back(t);
    }

    // その他
    this->mat = mat;
    isSingleSide = mat->refractive == 0;

    // トランスフォーム
    for (auto i = 0; i < vertNum; i++) {
        *vertices[i] = (*vertices[i] * scale).rotation(rot) + pos;
    }
#ifdef POLYGON_BVH
    // BVHを構築
    constructBVH();
#endif

}

PolygonObject::~PolygonObject()
{
    for (auto i = 0; i < vertNum; i++) {
        delete vertices[i];
    }
    delete vertices;
    delete verticesIndexes;
    delete mat;
    triangles.clear();
#ifdef POLYGON_BVH
    // BVH全ノードに対して責任を負う
    nodeList->clear();
#endif
}

void PolygonObject::intersect(const Ray &ray, Intersection &isect)
{
    Intersection nearest;
#ifdef POLYGON_BVH
    // 実際に交差を調べるポリゴンの候補を調べる
    // ルートノードのfindCandidatesを呼べば再帰的に調べてくれる
    auto candidates = nodeList->at(0)->findCandidates(ray);
    if (candidates.size() != 0) {
        auto a = 10;
        candidates = nodeList->at(0)->findCandidates(ray);
    }
    for (auto t : candidates) {
#else
    // 総当たりで最も近くで交差した点を求める
    for (auto t : triangles) {
#endif
        Intersection tmp;
        if (intersectTryangle(ray, *t.v[0], *t.v[1], *t.v[2], tmp) && tmp.t < nearest.t) {
            nearest = tmp;
        }
    }
    if (nearest.isHit()) {
        isect.t = nearest.t;
        *isect.mat = *mat;
        *isect.normal = *nearest.normal;
        *isect.point = *nearest.point;
    }
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

#ifdef POLYGON_BVH
void PolygonObject::constructBVH()
{
    nodeList = new vector<Bvh*>();
    new Bvh(triangles, *nodeList);
}

#endif