#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
    : width(width)
    , height(height)
    , colors(new Spectrum[width * height])
{
    for (auto i = 0; i < width * height; i++)
    {
        colors[i] = Spectrum::Black;
    }
}

Renderer::~Renderer()
{
    delete[] colors;
}

void Renderer::initScene()
{
    // ドラゴン
    scene.addObj(
        "resources\\Dragon.obj",
        Vec(),
        Vec(2.5),
        Vec(),
        Material(new Spectrum(1))
    );
    // 鏡
    scene.addIntersectable(new Sphere(
        0.5,
        new Vec(-4.5, 0.5, 2),
        new Material(new Spectrum(0.95, 0.6, 0.05), 1.0)
    ));
    // クリスタル
    scene.addIntersectable(new Sphere(
        1.0,
        new Vec(-2.0, 1.0, -1.0),
        new Material(new Spectrum(1), 0.05, 0.9, 2.8)
    ));
    // 赤
    scene.addIntersectable(new Sphere(
        0.5,
        new Vec(0.0, 0.5, 3.5),
        new Material(new Spectrum(Spectrum::Red), 0.9)
    ));
    // 緑
    scene.addIntersectable(new Sphere(
        0.5,
        new Vec(-4, 0.5, -4),
        new Material(new Spectrum(Spectrum::Green), 0.9)
    ));
    // 青
    scene.addIntersectable(new Sphere(
        1.0,
        new Vec(-1, 1.0, -7),
        new Material(new Spectrum(Spectrum::Blue), 0.9)
    ));
    // コーネルボックス
    // createCornellBox(6, 6, 8);

    // ライトの追加
#ifndef USE_PATH_TRACING
    scene.addLight(new Light(new Vec(2.5), new Spectrum(100)));
    scene.addLight(new Light(new Vec(-5.0, 2.0, -1), new Spectrum(80)));
    scene.addLight(new Light(new Vec(0.0, 2.5, 0.0), new Spectrum(250)));
    scene.addLight(new Light(new Vec(0.0, -2.5, 0.0), new Spectrum(30)));
#else
    scene.addIntersectable(new Sphere(
        1,
        new Vec(-8, 6, 3),
        new Material(new Spectrum(1, 1, 1), new Spectrum(30)),
        false
    ));
#endif
    // 無限平面
    scene.addIntersectable(new InfinitePlane(new Vec(0, 1, 0), 0,
        new Material(new Spectrum(0.99), 0.9)
    ));
}

void Renderer::initCubeMap()
{
    auto image = Image();
    int w, h;
    Spectrum **colors;
    for (auto i = 0; i < 6; i++) {
        image.loadPng("resources\\studio022_" + std::to_string(i) + ".png", colors, w, h);
        scene.addCubeMapImage(new CubeMapImage(colors, w, h), i);
    }
}

void Renderer::createCornellBox(double w, double h, double d)
{
    auto a = 1000.0;
    auto x = a + w / 2;
    auto y = a + h / 2;
    auto z = a + d / 2;
    scene.addIntersectable(new Sphere(a, new Vec(+x, 0, 0), new Material(new Spectrum(0, 0.95, 0), new Spectrum(0.1))));
    scene.addIntersectable(new Sphere(a, new Vec(-x, 0, 0), new Material(new Spectrum(0.95, 0, 0), new Spectrum(0.1))));
    scene.addIntersectable(new Sphere(a, new Vec(0, +y, 0), new Material(new Spectrum(0.95), new Spectrum(0.1))));
    scene.addIntersectable(new Sphere(a, new Vec(0, -y, 0), new Material(new Spectrum(0.95), new Spectrum(0.1))));
    scene.addIntersectable(new Sphere(a, new Vec(0, 0, -z), new Material(new Spectrum(0.95), new Spectrum(0.1))));
}

void Renderer::initTimer()
{
    lastPrintTime = getTime();
    lastSaveTime = getTime();
    startTime = getTime();
}

void Renderer::startRendering()
{
    // シーン準備
    initTimer();
    printf("Initializing scene...\n");
    initScene();
    initCubeMap();

    // カメラ作成
    auto aspect = (double)width / height;
    auto camPos = Vec(-7.5, 4, 7.25);
    auto cam = Camera(
        camPos,
        Vec(0.85, -0.5, -1),
        Vec(0, 1, 0),
        3.0,
        (camPos - Vec(0, 1, 1)).len(),
        0.4,
        aspect
    );

    // レイを飛ばして色を算出
    auto size = width * height;
    printf("tracing...\n");

    auto sampleIdx = 0;
    renderStartTime = getTime();

#ifdef PRODUCTION
    for (; getDiff(startTime, getTime()) < constants::kTIME_LIMIT; sampleIdx++) {
#pragma omp parallel for schedule(dynamic) num_threads(72)  // 競技用環境に合わせて論理コア数を直接指定
        for (auto y = 0; y < height; y++) {
            bindThread();  // 論理コアが64以上マシンでもCPUグループを跨いでスレッドを実行できるようにする
#else
    for (; sampleIdx < constants::kMAX_SAMPLING_NUM; sampleIdx++) {
#pragma omp parallel for schedule(dynamic) num_threads(11)
        for (auto y = 0; y < height; y++) {
#endif
            for (auto x = 0; x < width; x++) {
                auto idx = y * width + x;
                auto ray = cam.getPrimaryRay(
                    ((x + 0.5) / width - 0.5) * 2.0,
                    (-(y + 0.5) / height + 0.5) * 2.0,
                    width, height
                );
                scene.trace(ray, colors[idx]);
            }
        }
        checkProgress(sampleIdx + 1);
    }

    printf("Rendering Time: %s\n", timeFormat(getDiff(startTime, getTime())).c_str());
    printf("Start Saving Final Result\n");
#pragma omp parallel for
    for (auto i = 0; i < size; i++) {
        colors[i] = colors[i].scale(1.0 / (sampleIdx + 1));
    }       
    saveImage("output_final_result", colors);
    printf("Finalizing...\n");
}

void Renderer::checkProgress(int sampleNum)
{
    const double PrintInterval = 10000;
    auto now = getTime();
    auto printDiff = getDiff(lastPrintTime, now);
    if (printDiff > PrintInterval) {
        auto timeDiff = getDiff(startTime, now);
        auto prog = getProgress(sampleNum);
#ifdef PRODUCTION
        printf("%2.2lf%% Completed... Remaining %s\n", prog * 100.0, timeFormat(constants::kTIME_LIMIT - timeDiff).c_str());
#else
        int remainingTime = (timeDiff / prog - timeDiff);
        printf("%d / %d Samples  %3.5f Sec./iter.  Remaining: %s\n",
            sampleNum,
            constants::kMAX_SAMPLING_NUM,
            getDiff(renderStartTime, now) * 0.001 / sampleNum,
            timeFormat(remainingTime).c_str());
#endif
        lastPrintTime = now;
    }
    const double SaveIntervalTime = 15000;
    if (getDiff(lastSaveTime, now) > SaveIntervalTime) {
        auto saveColors = new Spectrum[width * height];
#pragma omp parallel for
        for (auto i = 0; i < width * height; i++) {
            saveColors[i] = colors[i].scale(1.0 / sampleNum);
        }
#pragma omp single
        {
#ifdef PRODUCTION
            saveImage("output", saveColors, false, sampleNum - 1);
#else
            saveImage("output", saveColors);
#endif
            delete[] saveColors;
            lastSaveTime = now;
        }
    }
}

void Renderer::saveImage(std::string fileName, Spectrum *colors, bool overWritten, int index)
{
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << index;
    fileName += overWritten ? "" : ("_" + ss.str());
    // ファイル用意
    Image image = Image();
    // 保存
    image.savePng(fileName, colors, width, height);
    printf("Saved.\n");
}

void Renderer::bindThread()
{
    auto thread_id = omp_get_thread_num();
    auto group = thread_id % 2; // 0, 1でプロセッサグループを指定
    GROUP_AFFINITY mask;
    if (GetNumaNodeProcessorMaskEx(group, &mask)) {
        SetThreadGroupAffinity(GetCurrentThread(), &mask, nullptr);
    }
}

double Renderer::getProgress(int sampleNum)
{;
    return sampleNum / (double)constants::kMAX_SAMPLING_NUM;
}

clk::time_point Renderer::getTime()
{
    return std::chrono::system_clock::now();
}

double Renderer::getDiff(clk::time_point start, clk::time_point end)
{
    return (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

std::string Renderer::timeFormat(double millisec)
{
    auto sec = (int)millisec / 1000;
    char buff[30];
    snprintf(buff, sizeof(buff), "%dH %dM %dS", sec / 3600, sec % 3600 / 60, sec % 3600 % 60);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}