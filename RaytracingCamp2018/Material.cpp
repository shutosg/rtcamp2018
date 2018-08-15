#include "Material.hpp"

Material::Material()
    : diffuse(new Spectrum(0.8))
    , reflective(0)
    , refractive(0)
    , refractiveIndex(1)
{
}

Material::Material(const Material &m)
    : diffuse(new Spectrum(*m.diffuse))
    , reflective(m.reflective)
    , refractive(m.refractive)
    , refractiveIndex(m.refractiveIndex)
{
}

Material::Material(Spectrum *diffuse, double reflective, double refractive, double refractiveIndex)
    : diffuse(diffuse)
    , reflective(reflective)
    , refractive(refractive)
    , refractiveIndex(refractiveIndex)
{
}

Material::~Material()
{
    delete diffuse;
}

void Material::operator=(const Material &m)
{
    *diffuse = *(m.diffuse);
    reflective = m.reflective;
    refractive = m.refractive;
    refractiveIndex = m.refractiveIndex;
}