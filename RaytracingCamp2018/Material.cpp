#include "Material.hpp"



Material::Material()
    : diffuse(new Spectrum(0.8))
{
}

Material::Material(const Material &material)
{
    diffuse = new Spectrum(
        material.diffuse->r,
        material.diffuse->g,
        material.diffuse->b
    );
}

Material::Material(Spectrum *diffuse)
    : diffuse(diffuse)
{
}

Material::~Material()
{
    delete diffuse;
}

void Material::operator=(const Material &m)
{
    *diffuse = *(m.diffuse);
}