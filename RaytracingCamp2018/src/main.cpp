#include "Renderer.hpp"

int main() {
#ifdef PRODUCTION
    const int w = 810;
    const int h = 540;
#else
    const int w = 480;
    const int h = 320;
#endif
    Renderer renderer = Renderer(w, h);
    renderer.startRendering();
    return 0;
}