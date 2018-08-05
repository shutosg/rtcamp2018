#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
{
    this->width = width;
    this->height = height;
}



void Renderer::startRendering()
{
    Ppm ppm = Ppm();
    printf("レンダリング開始\n");

    // 色の配列を準備
    std::vector<Vec> colors(width * height);
    for (int i = 0; i < width * height; i++) {
        colors[i] = Vec(255, 0, 0);
    }
    // 保存
    std::string fileName = "output.ppm";
    ppm.savePpm(fileName, colors, width, height);
    printf("レンダリング終了\n");
}