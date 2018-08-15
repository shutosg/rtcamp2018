#include "Renderer.hpp"

int main() {
    const int w = 480;
    const int h = 270;
    Renderer renderer = Renderer(w, h);
    renderer.startRendering();
    return 0;
}