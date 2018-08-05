#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <direct.h>
#include "Spectrum.hpp"

class Ppm
{
public:
    Ppm();
    void savePpm(std::string &fileName, Spectrum *colors, int width, int height);

private:
    const std::string saveFolder = std::string("render\\");
};

