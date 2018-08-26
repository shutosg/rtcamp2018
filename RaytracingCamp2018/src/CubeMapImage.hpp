#pragma once

#include "Spectrum.hpp"

struct CubeMapImage
{
    CubeMapImage()
        : pixels(NULL)
    {
    };
    CubeMapImage(Spectrum **pixels, int width, int height)
        : pixels(pixels)
        , width(width)
        , height(height)
    {
    };
    ~CubeMapImage()
    {
        for (auto i = 0; i < width * height; i++) {
            delete pixels[i];
        }
        delete[] pixels;
    };
    Spectrum **pixels;
    int width = 0;
    int height = 0;
};

enum Dir
{
    Right,
    Left,
    Top,
    Bottom,
    Back,
    Front
};