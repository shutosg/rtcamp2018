#pragma once

#include <iostream>
#include <fstream>
#include "Ppm.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "constant.h"

class Renderer
{
public:
    Renderer(int width, int height);
    void startRendering();

private:
    int width;
    int height;
    Scene scene;

    void initScene();
};

