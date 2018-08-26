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
    // 球の追加
    /*scene.addIntersectable(new Sphere(
        1,
        new Vec(0, -2, 0),
        new Material(new Spectrum(1, 1, 1), 0.1, 0.9, 1.5)
    ));*/
    /*scene.addIntersectable(new Sphere(
        0.8,
        new Vec(-2.0, 0.3, 0),
        new Material(new Spectrum(0.1, 0.5, 1), 0.9)
    ));*/
    scene.addIntersectable(new Sphere(1, new Vec(1.5, -1, -3.0), new Material(new Spectrum(1, 0.1, 1))));
    // scene.addIntersectable(new Sphere(1, new Vec(3, 0, 5.0), new Material(new Spectrum(0.1, 1, 0.1))));
    scene.addObj(
        "resources\\Triangle_01_a.obj",
        Vec(0, 1, 0),
        Vec(2.0),
        Vec(0, 40, 0),
        Material(new Spectrum(0.02), 0.8, 0.0, 1.0),
        4, Vec(0, 0.2, 0), Vec(), Vec(), true
    );
    scene.addObj(
        "resources\\Triangle_02.obj",
        Vec(0, 1, 0),
        Vec(2.0),
        Vec(0, 40, 0),
        Material(new Spectrum(1), 0.1, 0.9, 1.8)
    );
    scene.addObj(
        "resources\\Triangle_03.obj",
        Vec(0, 1, 0),
        Vec(2.0),
        Vec(0, 40, 0),
        Material(new Spectrum(1), new Spectrum(10))
    );
    scene.addIntersectable(new Sphere(
        0.25,
        new Vec(0, 1, 0),
        new Material(new Spectrum(1), new Spectrum(10))
    ));
    // コーネルボックス
    // createCornellBox(6, 6, 8);

    // ライトの追加
#ifndef USE_PATH_TRACING
    scene.addLight(new Light(new Vec(2.5), new Spectrum(100, 80, 80)));
    scene.addLight(new Light(new Vec(-1.0, -2.0, 2.5), new Spectrum(80, 80, 100)));
    scene.addLight(new Light(new Vec(0.0, 2.5, 0.0), new Spectrum(250)));
    scene.addLight(new Light(new Vec(0.0, -2.5, 0.0), new Spectrum(30)));
#else
    scene.addIntersectable(new Sphere(
        1,
        new Vec(0, 6, 0),
        new Material(new Spectrum(1, 1, 1), new Spectrum(50))
    ));
    scene.addIntersectable(new Sphere(
        1,
        new Vec(3, 1, 3),
        new Material(new Spectrum(1, 1, 1), new Spectrum(5)),
        false
    ));
#endif
    // 無限平面
    scene.addIntersectable(new InfinitePlane(new Vec(0, 1, 0), 0,
        new Material(new Spectrum(1), 0.5)
    ));
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
    lastSaveSampleNum = 0;
}

void Renderer::startRendering()
{
    // シーン準備
    initTimer();
    printf("Initializing scene...\n");
    scene = Scene();
    initScene();

    // カメラ作成
    auto aspect = (double)width / height;
    auto cam = Camera(
        Vec(0, 5, 9),
        Vec(0, -0.275, -0.5),
        Vec(0, 1, 0),
        1.5,
        9.5,
        0.05,
        aspect
    );

    // レイを飛ばして色を算出
    auto size = width * height;
    printf("tracing...\n");
    Random& rnd = Random::get_instance();

    auto sampleIdx = 0;

    for (; getDiff(startTime, getTime()) < constants::kTIME_LIMIT; sampleIdx++) {
#pragma omp parallel for schedule(dynamic) num_threads(72)  // 競技用環境に合わせて論理コア数を直接指定
        for (auto y = 0; y < height; y++) {
            bindThread();  // 論理コアが64以上マシンでもCPUグループを跨いでスレッドを実行できるようにする
            for (auto x = 0; x < width; x++) {
                auto idx = y * width + x;
                auto ray = cam.getPrimaryRay((x + rnd.random(-0.5, 0.5)) / width - 0.5, -(y + rnd.random(-0.5, 0.5)) / height + 0.5);
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
        auto prog = getProgress(sampleNum);
        auto timeDiff = getDiff(startTime, now);
        int remainingTime = (timeDiff / prog - timeDiff);
        printf("%2.2lf%% Completed... Remaining %s\n", prog * 100.0, timeFormat(remainingTime).c_str());
        lastPrintTime = now;
    }
    const double SaveIntervalTime = 15000;
    const int SaveIntervalSampleNum = 9999999999;
    if (getDiff(lastSaveTime, now) > SaveIntervalTime || sampleNum - lastSaveSampleNum >= SaveIntervalSampleNum) {
        auto saveColors = new Spectrum[width * height];
#pragma omp parallel for
        for (auto i = 0; i < width * height; i++) {
            saveColors[i] = colors[i].scale(1.0 / sampleNum);
        }
#pragma omp critical
        {
            saveImage("output", saveColors, false, sampleNum - 1);
            // saveImage("output", saveColors);
            delete[] saveColors;
            lastSaveTime = now;
            lastSaveSampleNum = sampleNum;
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
    snprintf(buff, sizeof(buff), "%d Hour %d Min. %d Sec.", sec / 3600, sec % 3600 / 60, sec % 3600 % 60);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}