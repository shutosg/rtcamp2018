﻿#include "Scene.hpp"

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
    for (auto i : cubeMapImages) {
        if (i) delete i;
    }
}


void Scene::addIntersectable(IIntersectable *obj)
{
    objs->push_back(obj);
}

// Objを読み込んでシーンに追加（クローンのオプション指定可能）
void Scene::addObj(
    const std::string fileName,
    const Vec &pos,
    const Vec &scale,
    const Vec &rot,
    const Material &mat,
    const int cloneNum, const Vec posOffset, const Vec scaleOffset, const Vec rotOffset, bool scaleReflect)
{
    ObjLoader obj(fileName);
    for (auto i = 0; i < cloneNum; i++) {
        addIntersectable(new PolygonObject(
            obj,
            pos + posOffset.scale(i),
            scale * (Vec(1) + (scaleReflect && i < 0 ? posOffset.flag().scale(-2) : Vec(0))) + scaleOffset.scale(i),
            rot + rotOffset.scale(i),
            new Material(mat)
        ));
    }
}

void Scene::addLight(Light *light)
{
    lights->push_back(light);
}

void Scene::addCubeMapImage(CubeMapImage *image, int dir)
{
    cubeMapImages[dir] = image;
}

void Scene::trace(const Ray &ray, Spectrum &spectrum, int depth)
{
    // 反射制限数チェック
    if (depth >= constants::kMAX_TRACE_DEPTH) return;

    auto nearest = Intersection();
    findNearestInterSection(ray, depth, nearest);
    // ヒットしなかったので空の色
    if (!nearest.isHit()) {
        addSkyColor(ray, spectrum);
        return;
    }

    // 物体の外部からの交差
    if (nearest.normal.dot(ray.dir) < 0) {
        intersectSurface(ray.dir, nearest, spectrum, constants::kVACUUM_REFRACTIVE_INDEX / nearest.mat->refractiveIndex, depth);
        if (!nearest.mat->emission->equals(Spectrum::Black)) {
            spectrum += nearest.mat->emission->scale(-nearest.normal.dot(ray.dir));
        }
    } else {
        // 物体の内部からの交差
        // 屈折レイ
        nearest.normal = nearest.normal.neg();
        intersectSurface(ray.dir, nearest, spectrum, nearest.mat->refractiveIndex / constants::kVACUUM_REFRACTIVE_INDEX, depth);
    }
}


void Scene::intersectSurface(const Vec &dir, const Intersection &isect, Spectrum &spectrum, double eta, int depth)
{
    // トレース数削減のため、鏡面、屈折、拡散のどれか一つだけトレースする
    auto t = Random::get_instance().random();
    auto ks = isect.mat->reflective;
    auto kt = isect.mat->refractive;
    auto kd = 1.0 - ks - kt;
    if (t < ks) {
        // 鏡面反射
        auto reflect = dir.reflect(isect.normal);
        Spectrum s(Spectrum::Black);
        trace(Ray(isect.point, reflect.normalize(), true), s, depth + 1);
        spectrum += s.scale(ks) * *isect.mat->diffuse;
    } else if (t < ks + kt) {
        // 屈折
        auto refract = dir.refract(isect.normal, eta);
        Spectrum s(Spectrum::Black);
        trace(Ray(isect.point, refract.normalize(), true), s, depth + 1);
        spectrum += s.scale(kt) * *isect.mat->diffuse;
    } else {
        // 拡散反射
#ifdef USE_PATH_TRACING
        Spectrum s(Spectrum::Black);
        auto v = isect.normal.cosineWeightedRandomHemisphere();
        trace(Ray(isect.point, v, true), s, depth + 1);
        auto fr = isect.mat->diffuse->scale((1.0 - ks - kt) / constants::kPI);
        double factor = 2.0 * constants::kPI * isect.normal.dot(v);
        spectrum += (s * fr).scale(factor);
#else
        for (auto i = 0; i < lights->size(); i++) {
            diffuseLighting(*isect.point, *isect.normal, *lights->at(i), *isect.mat->diffuse, spectrum);
        }
#endif
    }
}

void Scene::findNearestInterSection(const Ray &ray, int depth, Intersection &nearest)
{
    for (auto i = 0; i < objs->size(); i++) {
        auto result = Intersection();
        objs->at(i)->intersect(ray, depth, result);
        if (result.t < nearest.t) {
            nearest = result;
        }
    }
}

void Scene::diffuseLighting(const Vec &p, const Vec &n, const Light &light, const Spectrum &matDiffuse, Spectrum &spectrum)
{
    // 交点から光源に向かうベクトル
    auto v = (light.pos - p);
    auto dot = n.dot(v.normalize());
    // 面が光源を向いている
    if (dot > 0) {
        // 遮蔽物チェック
        if (!visible(p, light.pos)) return;
        // 光源からの距離に応じて拡散反射ライティング
        auto r = v.len();
        spectrum += (light.power).scale(dot / (4 * constants::kPI * r * r)) * matDiffuse;
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
        obj->intersect(ray, 0, isect);
        // posから光源までの間に遮蔽物がある
        if (isect.t < v.len()) return false;
    }
    return true;
}

void Scene::addSkyColor(const Ray &ray, Spectrum &spectrum)
{
    if (cubeMapImages[0] && cubeMapImages[0]->width != 0) {
        // キューブマップが有効なので環境色を算出

        // x,y,zのどの方向を見ているか
        auto lookDirAxis = ray.dir.abs().maxAxis();
        double dir[] = {ray.dir.x, ray.dir.y, ray.dir.z};
        // 最大の要素が1のdir
        auto tmp = ray.dir.scale(1.0 / std::abs(dir[lookDirAxis]));
        Vec uv;
        // uv座標[-1, 1]を算出
        switch (lookDirAxis)
        {
        case 0:
            uv.x = tmp.z;
            uv.y = tmp.y;
            if (ray.dir.x < 0)
                uv.x *= -1;
            break;
        case 1:
            uv.x = tmp.x;
            uv.y = tmp.z;
            if (ray.dir.y < 0)
                uv.x *= -1;
            break;
        case 2:
            uv.x = tmp.x;
            uv.y = tmp.y;
            if (ray.dir.z > 0)
                uv.x *= -1;
            break;
        }
        // 0:right, 1:left, 2:top, 3:bottom, 4:back, 5:front
        auto imageIdx = lookDirAxis * 2 + (dir[lookDirAxis] < 0 ? 1 : 0);
        auto image = cubeMapImages[imageIdx];
        // [-1, 1]のuvを[0, width-1]に変換
        auto w = (int)((uv.x + 1) * 0.5 * (image->width - 1));
        auto h = (int)((uv.y * -1 + 1) * 0.5 * (image->height - 1));
        spectrum += *image->pixels[image->width * h + w];
    }
    else {
        spectrum += Spectrum::Sky;
    }
}
