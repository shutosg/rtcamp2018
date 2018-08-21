#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "Image.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "constant.h"
#include "ObjLoader.hpp"

using clk = std::chrono::system_clock;

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();
    void startRendering();

private:
    int width;
    int height;
    Scene scene;
    Spectrum *colors;
    clk::time_point startTime;
    clk::time_point lastPrintTime;
    clk::time_point lastSaveTime;
    int lastSaveSampleNum;

    void initScene();
    void initTimer();
    void saveImage(std::string fileName, Spectrum * colors, bool overWritten = true, int index = -1);
    void createCornellBox(double w, double h, double d);
    double getProgress(int sampleIdx);
    void checkProgress(int sampleIdx);
    clk::time_point getTime();
    double getDiff(clk::time_point start, clk::time_point end);
};

