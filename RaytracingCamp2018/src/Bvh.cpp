#include "Bvh.hpp"

Bvh::Bvh()
    : aabbox(new AABBox())
{
    childs[0] = NULL;
    childs[1] = NULL;
}

Bvh::Bvh(const std::vector<Triangle> &tris, std::vector<Bvh*> &nodeList) : Bvh()
{
    for (auto t : tris) {
        this->tris.push_back(t);
        for (auto i = 0; i < 3; i++) {
            aabbox->minPoint = aabbox->minPoint.min(*t.v[i]);
            aabbox->maxPoint = aabbox->maxPoint.max(*t.v[i]);
        }
    }
    nodeList.push_back(this);
    // 条件を満たしているなら更に分割する
    const int minTriangleNum = 2;
    if (this->tris.size() > minTriangleNum) {
        // trisを分割して葉を作る
        std::vector<Triangle> lLeaf;
        std::vector<Triangle> rLeaf;
        for (auto i = 0; i < tris.size(); i++) {
            if (i <= tris.size() / 2) {
                lLeaf.push_back(tris[i]);
            } else {
                rLeaf.push_back(tris[i]);
            }
        }
        this->tris.clear();
        // ここでnewしたBVHはPolygonObjectが責任を負う
        childs[0] = new Bvh(lLeaf, nodeList);
        if (rLeaf.size() != 0) {
            childs[1] = new Bvh(rLeaf, nodeList);
        }
    }
}

Bvh::Bvh(const Bvh &bvh)
{
    aabbox = new AABBox(*bvh.aabbox);
    childs[0] = bvh.childs[0];
    childs[1] = bvh.childs[1];
    for (auto t : bvh.tris) {
        tris.push_back(t);
    }
}

Bvh::~Bvh()
{
    delete[] childs;
    delete aabbox;
}

std::vector<Triangle> Bvh::findCandidates(const Ray &ray) const
{
    std::vector<Triangle> candidates;
    Intersection isect;
    aabbox->intersect(ray, isect);
    // 自身のAABBにヒットしなければ空で返す
    if (!isect.isHit()) {
        return candidates;
    }
    // 自身が葉ノードなら含まれるポリゴンを返す
    if (tris.size() != 0) {
        return tris;
    }

    // 子を調べる
    for (auto c : childs) {
        if (!c) return candidates;
        auto tmp = c->findCandidates(ray);
        for (auto t : tmp) {
            candidates.push_back(t);
        }
    }
    return candidates;
}

std::string Bvh::toStr() const
{
    std::string header = "Bvh:\n";
    std::string minMax = "    minPoint = " + aabbox->minPoint.toStr() + ", maxPoint = " + aabbox->maxPoint.toStr() + "\n";
    std::string child  = "    childIds (";
    child = child + (childs[0] ? "not NULL" : "") + ", " + (childs[1] ? "not NULL" : "") + ")\n";
    std::string vertex = tris.size() == 0 ? "" : "    trisNum = " +  std::to_string(tris.size()) + "\n";
    return header + minMax + child + vertex;
}