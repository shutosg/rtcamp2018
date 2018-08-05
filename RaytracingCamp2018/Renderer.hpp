#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Ppm.hpp"

class Renderer
{
public:
    Renderer(int width, int height);
    void startRendering();

private:
    int width;
    int height;
};

