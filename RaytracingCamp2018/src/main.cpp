#include "Renderer.hpp"

int main() {
    const int w = 480;
    const int h = 270;
    // const int w = 1280;
    // const int h = 720;
    Renderer renderer = Renderer(w, h);
    renderer.startRendering();
    return 0;
}