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
    void savePpm(std::string &fileName, Spectrum *colors, int width, int height);
    void savePng(std::string &fileName, Spectrum *colors, int width, int height);

private:
    const std::string saveFolder = std::string("render\\");
    void checkDirectory();
};

