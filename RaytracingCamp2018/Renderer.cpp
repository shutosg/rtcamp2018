#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
    : width(width)
    , height(height)
{
}

void Renderer::initScene()
{
    scene.addIntersectable(
        new Sphere(0.25, new Vec(0, 0, 0), new Material(new Spectrum(1, 1, 1)))
    );
    scene.addIntersectable(
        new Sphere(0.1, new Vec(1, 0, 0), new Material(new Spectrum(1, 0, 0)))
    );
}

void Renderer::startRendering()
{
    // シーン準備
    printf("シーン準備\n");
    scene = Scene();
    initScene();

    // カメラ作成
    auto aspect = (double)width / height;
    auto cam = Camera(
        Vec(0, 0, 10),
        Vec(0, 0, -1),
        Vec(0, 1, 0),
        10,
        aspect
    );

    // レイを飛ばして色を算出
    Spectrum *colors = new Spectrum[width * height];
    printf("tracing...\n");
    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
            auto idx = y * width + x;
            colors[idx] = Spectrum::Black;
            auto ray = cam.getPrimaryRay((x + 0.5) / width - 0.5, -(y + 0.5) / height + 0.5);
            scene.trace(ray, colors[idx]);
        }
    }

    // ファイル用意
    Ppm ppm = Ppm();
    printf("保存中\n");
    // 保存
    std::string fileName = "output.ppm";
    ppm.savePpm(fileName, colors, width, height);
    delete colors;
    printf("終了\n");
}
