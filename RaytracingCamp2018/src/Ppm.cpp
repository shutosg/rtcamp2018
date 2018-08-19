#include "Ppm.hpp"

Ppm::Ppm()
{
}

void Ppm::savePpm(std::string &fileName, Spectrum *colors, int width, int height) {
    struct stat statBuf;
    const char *dir = saveFolder.c_str();

    // 保存パスの存在チェック
    if (stat(dir, &statBuf) != 0) {
        // ディレクトリ作成
        int result = _mkdir(dir);
        if (result != 0) {
            printf("ディレクトリの作成に失敗しました\n");
            return;
        }
        printf("ディレクトリを作成しました\n");
    }

    // 保存
    std::ofstream ofs((saveFolder + fileName));
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; i++) {
        auto col = colors[i].toColor();
        ofs << col.x << " " << col.y << " " << col.z << "\n";
    }
}