#include "Image.hpp"

#include "../external/stb/stb_image_write.h"
#include "../external/stb/stb_image.h"

Image::Image()
{
}

void Image::savePpm(const std::string &fileName, Spectrum *colors, int width, int height)
{
    checkDirectory();

    // 保存
    std::ofstream ofs((saveFolder + fileName + ".ppm"));
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; i++) {
        auto col = colors[i].toColor();
        ofs << col.x << " " << col.y << " " << col.z << "\n";
    }
}

void Image::savePng(const std::string &fileName, Spectrum *colors, int width, int height)
{
    checkDirectory();

    std::vector<uint8_t> bytes;
    bytes.resize(width * height * 3);

    for (int i = 0; i < width * height; i++) {
        auto col = colors[i].toColor();
        bytes.at(i * 3 + 0) = (uint8_t)col.x;
        bytes.at(i * 3 + 1) = (uint8_t)col.y;
        bytes.at(i * 3 + 2) = (uint8_t)col.z;
    }

    stbi_write_png((saveFolder + fileName + ".png").c_str(), width, height, 3, bytes.data(), width * 3);
}

void Image::loadPng(const std::string &fileName, Spectrum **&colors, int &width, int &height)
{
    uint8_t *pixels;
    int comp;
    pixels = stbi_load(fileName.c_str(), &width, &height, &comp, 3);
    colors = new Spectrum*[width * height];
    for (int i = 0; i < width * height; i++) {
        colors[i] = new Spectrum(
            pixels[i * 3 + 0] / 255.0,
            pixels[i * 3 + 1] / 255.0,
            pixels[i * 3 + 2] / 255.0
        );
    }
    stbi_image_free(pixels);
}

void Image::checkDirectory()
{
    struct stat statBuf;
    const char *dir = saveFolder.c_str();

    // 保存パスの存在チェック
    if (stat(dir, &statBuf) != 0) {
        // ディレクトリ作成
        int result = _mkdir(dir);
        if (result != 0) {
            printf("Failed to make directory.\n");
            return;
        }
        printf("Successfully created save directory.\n");
    }
}