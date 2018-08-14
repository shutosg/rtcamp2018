#include "Renderer.hpp"

int main() {
    const int w = 1200;
    const int h = 800;
    Renderer renderer = Renderer(w, h);
    renderer.startRendering();
    return 0;
}