#include "Material.hpp"

Material::Material()
    : Material(new Spectrum(0.8), new Spectrum(0), 0, 0, 1)
{
}

Material::Material(const Material &m)
    : Material(
        new Spectrum(*m.diffuse),
        new Spectrum(*m.emission),
        m.reflective,
        m.refractive,
        m.refractiveIndex)
{
}

Material::Material(Spectrum *diffuse, double reflective, double refractive, double refractiveIndex)
    : Material(
        diffuse,
        new Spectrum(0),
        reflective,
        refractive,
        refractiveIndex)
{
}

Material::Material(Spectrum *diffuse, Spectrum *emission, double reflective, double refractive, double refractiveIndex)
    : diffuse(diffuse)
    , emission(emission)
    , reflective(reflective)
    , refractive(refractive)
    , refractiveIndex(refractiveIndex)
{
}

Material::~Material()
{
    delete diffuse;
    delete emission;
}

void Material::operator=(const Material &m)
{
    *diffuse = *(m.diffuse);
    *emission = *(m.emission);
    reflective = m.reflective;
    refractive = m.refractive;
    refractiveIndex = m.refractiveIndex;
}