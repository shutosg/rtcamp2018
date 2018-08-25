#include "Bvh.hpp"

Bvh::Bvh()
    : aabbox(new AABBox())
{
    childs[0] = NULL;
    childs[1] = NULL;
}

Bvh::Bvh(const std::vector<Triangle> &tris, std::vector<Bvh*> &nodeList) : Bvh()
{
    nodeList.push_back(this);
    for (auto t : tris) {
        this->tris.push_back(t);
        for (auto i = 0; i < 3; i++) {
            aabbox->minPoint = aabbox->minPoint.min(*t.v[i]);
            aabbox->maxPoint = aabbox->maxPoint.max(*t.v[i]);
        }
    }
    // SAHに基づいて更に分割するべきか調べる
    // 参考：https://qiita.com/omochi64/items/9336f57118ba918f82ec#bvh-%E3%81%AE%E6%A7%8B%E7%AF%89
    auto bestAxis = -1;
    auto bestIndex = -1;
    // 2つの枝に分割するときに、trisの0～bestIndexまでのインデックスのポリゴンが一つの枝に含まれる
    findBestSplit(tris, bestAxis, bestIndex);
    // 条件を満たしているなら更に分割する
    if (bestAxis != -1 && bestIndex != -1) {
        std::vector<Triangle> lLeaf(tris);
        std::vector<Triangle> rLeaf;
        // bestAxisでソート
        sortTriangle(lLeaf, bestAxis);
        // bestIndexで分割
        for (auto i = lLeaf.size() - 2; i > 0; i--) {
            rLeaf.push_back(lLeaf.back());
            lLeaf.pop_back();
            if (i == bestIndex) break;
        }
        // ここでnewしたBVHはPolygonObjectが責任を負う
        childs[0] = new Bvh(lLeaf, nodeList);
        if (rLeaf.size() != 0) {
            childs[1] = new Bvh(rLeaf, nodeList);
        }
        this->tris.clear();
    }
}

// コストを調べるために一時的なBvhを生成する用
Bvh::Bvh(std::vector<Triangle> tris) : Bvh()
{
    for (auto t : tris) {
        this->tris.push_back(t);
        for (auto i = 0; i < 3; i++) {
            aabbox->minPoint = aabbox->minPoint.min(*t.v[i]);
            aabbox->maxPoint = aabbox->maxPoint.max(*t.v[i]);
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
    if (childs[0]) delete childs[0];
    if (childs[1]) delete childs[1];
    delete aabbox;
}

void Bvh::findBestSplit(std::vector<Triangle> tris, int &bestAxis, int &bestIndex)
{
    // コスト変数を、非分割時をベストとして初期化する
    double bestCost = CostTriangleIntersection * tris.size();
    double SARoot = aabbox->getSurfaceArea();
    // bestAxisとbestIndexを調べる
    // 0:x 1:y 2:z
    for (auto axis = 0; axis < 3; axis++) {
        // 指定した軸方向の座標でソート
        sortTriangle(tris, axis);
        // 2領域に分割してコストを計算する
        std::vector<Triangle> tris2;
        auto ahhhh = tris.size();
        for (int index = tris.size() - 2; index >= 0; index--) {
            tris2.push_back(tris.back());
            tris.pop_back();
            if (tris2.size() <= 1) continue;

            Bvh bvhA(tris);
            Bvh bvhB(tris2);
            // SAHコスト算出
            auto cost = 2.0 * CostAABBIntersection
                + (bvhA.aabbox->getSurfaceArea() * bvhA.tris.size() + bvhB.aabbox->getSurfaceArea() * bvhB.tris.size())
                * CostTriangleIntersection / SARoot;
            // コストが最小ならbest更新
            if (cost < bestCost) {
                bestAxis = axis;
                bestIndex = index;
                bestCost = cost;
            }
        }
    }
}

std::vector<Triangle> Bvh::findCandidates(const Ray &ray) const
{
    std::vector<Triangle> candidates;
    Intersection isect;
    aabbox->intersect(ray, 0, isect);
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

void Bvh::sortTriangle(std::vector<Triangle> &tris, int axis)
{
    std::sort(tris.begin(), tris.end(),
        [axis](const Triangle a, const Triangle b) {
        switch (axis)
        {
        case 0:
            return a.center.x < b.center.x;
        case 1:
            return a.center.y < b.center.y;
        case 2:
            return a.center.z < b.center.z;
        default:
            break;
        }
    });
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
