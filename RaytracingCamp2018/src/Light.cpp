#include "Light.hpp"

Light::Light()
    : pos(new Vec(1.0))
    , power(new Spectrum(10))
{
}

Light::Light(const Light &l)
{
    pos = new Vec(*l.pos);
    power = new Spectrum(*l.power);
}

Light::Light(Vec *pos, Spectrum *power)
    : pos(pos)
    , power(power)
{
}


Light::~Light()
{
    delete pos;
    delete power;
}
