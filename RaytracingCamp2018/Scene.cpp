#include "Scene.hpp"

Scene::Scene()
    : objs(new std::vector<IIntersectable*>())
{
}


Scene::~Scene()
{
    for (IIntersectable* element : *objs)
    {
        delete element;
    }
    objs->clear();
}


void Scene::addIntersectable(IIntersectable *obj)
{
    objs->push_back(obj);
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
    spectrum += *nearest.mat->diffuse;
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