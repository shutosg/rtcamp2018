#pragma once

#include <random>

class Random
{
private:
    Random();
    ~Random();
    std::mt19937 *mt;
    std::uniform_real_distribution<double> *dist;
public:
    Random(const Random&) = delete;
    Random& operator = (const Random&) = delete;
    Random(Random&&) = delete;
    Random& operator = (Random&&) = delete;
    static Random& get_instance();
    double random();
    double random(double from, double to);
};

