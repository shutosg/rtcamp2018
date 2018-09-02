#include "Renderer.hpp"

int main() {
#ifdef PRODUCTION
    const int w = 1620;
    const int h = 1080;
#else
    const int w = 480;
    const int h = 320;
#endif
    Renderer renderer = Renderer(w, h);
    renderer.startRendering();
    return 0;
}