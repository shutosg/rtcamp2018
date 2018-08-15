#include "Scene.hpp"

Scene::Scene()
    : objs(new std::vector<IIntersectable*>())
    , lights(new std::vector<Light*>())
{
}


Scene::~Scene()
{
    for (IIntersectable* element : *objs) {
        delete element;
    }
    objs->clear();

    for (Light* element : *lights) {
        delete element;
    }
    lights->clear();
}


void Scene::addIntersectable(IIntersectable *obj)
{
    objs->push_back(obj);
}

void Scene::addLight(Light *light)
{
    lights->push_back(light);
}

void Scene::trace(const Ray &ray, Spectrum &spectrum)
{
    auto nearest = Intersection();
    findNearestInterSection(ray, nearest);
    // ヒットしなかったので空の色
    if (!nearest.isHit()) {
        spectrum += Spectrum::Sky;
        return;
    }
    // 物体にヒットしたのでライトの数だけライティング
    for (auto i = 0; i < lights->size(); i++) {
        diffuseLighting(*nearest.point, *nearest.normal, *lights->at(i), *nearest.mat->diffuse, spectrum);
    }
}

void Scene::findNearestInterSection(const Ray &ray, Intersection &nearest)
{
    for (auto i = 0; i < objs->size(); i++) {
        auto result = Intersection();
        objs->at(i)->intersect(ray, result);
        if (result.t < nearest.t) {
            nearest = result;
        }
    }
}

void Scene::diffuseLighting(const Vec &p, const Vec &n, const Light &light, const Spectrum &matDiffuse, Spectrum &spectrum)
{
    // 交点から光源に向かうベクトル
    auto v = (*light.pos - p);
    auto dot = n.dot(v.normalize());
    // 面が光源を向いている
    if (dot > 0) {
        // 遮蔽物チェック
        if (!visible(p, *light.pos)) return;
        // 光源からの距離に応じて拡散反射ライティング
        auto r = v.len();
        spectrum += (*light.power).scale(dot / (4 * constants::kPI * r * r)) * matDiffuse;
    }
}

bool Scene::visible(const Vec &from, const Vec &to)
{
    // posから光源に向かうベクトル
    auto v = (to - from).normalize();
    auto ray = Ray(from, v, true);
    for (auto i = 0; i < objs->size(); i++) {
        auto obj = objs->at(i);
        auto isect = Intersection();
        obj->intersect(ray, isect);
        // posから光源までの間に遮蔽物がある
        if (isect.t < v.len()) return false;
    }
    return true;
}