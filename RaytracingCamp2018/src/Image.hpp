#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <direct.h>
#include "Spectrum.hpp"

class Image
{
public:
    Image();
    void savePpm(const std::string &fileName, Spectrum *colors, int width, int height);
    void savePng(const std::string &fileName, Spectrum *colors, int width, int height);
    void loadPng(const std::string &fileName, Spectrum **&colors, int &width, int &height);

private:
    const std::string saveFolder = std::string("render\\");
    void checkDirectory();
};

