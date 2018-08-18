#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
    : width(width)
    , height(height)
{
}

void Renderer::initScene()
{
    // 球の追加
    scene.addIntersectable(new Sphere(
        1,
        new Vec(0, -2, 0.5),
        new Material(new Spectrum(1, 1, 1), 0.3, 0.7, 1.6)
    ));
    scene.addIntersectable(new Sphere(
        0.8,
        new Vec(-2.0, 0.3, 0.1),
        new Material(new Spectrum(1, 1, 1), 0.8)
    ));
    scene.addIntersectable(new Sphere(1, new Vec(1.5, -2, -1.0), new Material(new Spectrum(0.1, 0.1, 1))));

    // コーネルボックス
    createCornellBox(6, 6, 8);

    // ライトの追加
    // scene.addLight(new Light(new Vec(2.5), new Spectrum(100, 80, 80)));
    // scene.addLight(new Light(new Vec(-1.0, -2.0, 2.5), new Spectrum(80, 80, 100)));
    scene.addLight(new Light(new Vec(0.0, 2.5, 0.0), new Spectrum(250)));
    scene.addLight(new Light(new Vec(0.0, -2.5, 0.0), new Spectrum(30)));

    // 無限平面
    // scene.addIntersectable(new Plane(new Vec(0, 1, 0), 1.5, new Material()));
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
        1.5,
        aspect
    );

    // レイを飛ばして色を算出
    Spectrum *colors = new Spectrum[width * height];
    printf("tracing...\n");
    auto lastPrint = 0.0;
    auto size = width * height;

#pragma omp parallel for schedule(dynamic)
    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
            auto idx = y * width + x;
            // OpenMP無効時のみ
            #ifndef _OPENMP
            if (width > 480 && (double)idx / size > lastPrint + 0.1) {
                printf("%2.2lf%%完了…\n", (double)idx / size * 100.0);
                lastPrint += 0.1;
            }
            #endif

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
