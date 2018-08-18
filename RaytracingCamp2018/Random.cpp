#include "Random.hpp"

Random::Random()
    : mt(new std::mt19937(std::random_device()()))
    , dist(new std::uniform_real_distribution<double>(0.0, 1.0))
{
}

Random::~Random()
{
    delete mt;
    delete dist;
}

Random & Random::get_instance()
{
    static Random instance;
    return instance;
}

double Random::random()
{
    return (*dist)(*mt);
}

double Random::random(double from, double to)
{
    return from + (to - from) * (*dist)(*mt);
}
