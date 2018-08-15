#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
    : width(width)
    , height(height)
{
}

void Renderer::initScene()
{
    // 球の追加
    scene.addIntersectable(new Sphere(0.25, new Vec(0), new Material(new Spectrum(1, 1, 1))));
    scene.addIntersectable(new Sphere(0.3, new Vec(-0.5, -0.3, 0.4), new Material(new Spectrum(1, 1, 1))));
    scene.addIntersectable(new Sphere(0.3, new Vec(0.3, 0.3, -0.4), new Material(new Spectrum(1, 1, 1))));

    // コーネルボックス
    createCornellBox(6, 6, 8);

    // ライトの追加
    scene.addLight(new Light(new Vec(1.95), new Spectrum(100, 80, 80)));
    scene.addLight(new Light(new Vec(-1.0, -1.95, 1.95), new Spectrum(80, 80, 100)));
}

void Renderer::createCornellBox(double w, double h, double d)
{
    auto a = 1000.0;
    auto x = a + w / 2;
    auto y = a + h / 2;
    auto z = a + d / 2;
    scene.addIntersectable(new Sphere(a, new Vec(+x, 0, 0), new Material(new Spectrum(0, 0.95, 0))));
    scene.addIntersectable(new Sphere(a, new Vec(-x, 0, 0), new Material(new Spectrum(0.95, 0, 0))));
    scene.addIntersectable(new Sphere(a, new Vec(0, +y, 0), new Material(new Spectrum(0.95))));
    scene.addIntersectable(new Sphere(a, new Vec(0, -y, 0), new Material(new Spectrum(0.95))));
    scene.addIntersectable(new Sphere(a, new Vec(0, 0, -z), new Material(new Spectrum(0.95))));
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
        2,
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
