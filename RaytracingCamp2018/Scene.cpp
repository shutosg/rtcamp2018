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

void Scene::trace(const Ray &ray, Spectrum &spectrum, int depth)
{
    // 反射制限数チェック
    if (depth >= constants::kMAX_TRACE_DEPTH) return;

    auto nearest = Intersection();
    findNearestInterSection(ray, nearest);
    // ヒットしなかったので空の色
    if (!nearest.isHit()) {
        spectrum += Spectrum::Sky;
        return;
    }

    // 物体の外部からの交差
    if (nearest.normal->dot(ray.dir) < 0) {
        // 鏡面反射
        auto ks = nearest.mat->reflective;
        if (ks > 0) {
            // 反射レイ
            auto reflect = ray.dir.reflect(*nearest.normal);
            Spectrum s(Spectrum::Black);
            trace(Ray(*nearest.point, reflect.normalize(), true), s, depth + 1);
            spectrum += s.scale(ks) * *nearest.mat->diffuse;
        }
        // 屈折
        auto kt = nearest.mat->refractive;
        if (kt > 0) {
            // 屈折レイ
            auto refract = ray.dir.refract(*nearest.normal, constants::kVACUUM_REFRACTIVE_INDEX / nearest.mat->refractiveIndex);
            Spectrum s(Spectrum::Black);
            trace(Ray(*nearest.point, refract.normalize(), true), s, depth + 1);
            spectrum += s.scale(kt) * *nearest.mat->diffuse;
        }
        // 拡散反射
        auto kd = 1.0 - ks - kt;
        if (kd > 0) {
            Spectrum s(Spectrum::Black);
            for (auto i = 0; i < lights->size(); i++) {
                diffuseLighting(*nearest.point, *nearest.normal, *lights->at(i), *nearest.mat->diffuse, s);
            }
            spectrum += s.scale(kd);
        }
    } else {
        // 物体の内部からの交差
        // 屈折レイ
        auto refract = ray.dir.refract(nearest.normal->neg(), nearest.mat->refractiveIndex / constants::kVACUUM_REFRACTIVE_INDEX);
        trace(Ray(*nearest.point, refract.normalize(), true), spectrum, depth + 1);
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