#include "ObjLoader.hpp"

ObjLoader::ObjLoader()
{
}

ObjLoader::ObjLoader(const string &fileName)
    : vertices(new vector<Vec*>())
    , faceIndexes(new vector<int>())
    , verticesIndexes(new vector<int>())
{
    ifstream ifs(fileName);
    string buffer;

    while (ifs && getline(ifs, buffer)) {
#ifdef OBJLOADER_PRINT_DEBUG
        cout << "Reading file line: " << buffer << endl;
#endif
        vector<string> words = util::split(buffer, ' ');
        if (words.size() == 0) continue;
        if (words.at(0) == "v") {
            // vertex
            vertices->push_back(new Vec(stod(words.at(1)), stod(words.at(2)), stod(words.at(3))));
        } else if (words.at(0) == "vn") {
            // normal
            continue;
        } else if (words.at(0) == "f") {
            // vertex, uv, normalのindex
            // NOTE: 3角ポリゴンのみ対応
            for (auto i = 1; i < 4; i++) {
                vector<string> v = util::split(words.at(i), '/');
                verticesIndexes->push_back(stoi(v.at(0)));
                // uvIndexes->push_back(stod(v.at(1)));  uv
                // normalIndexes->push_back(stod(v.at(2)));  normal
            }
            faceIndexes->push_back(3);
        }
    }
#ifdef OBJLOADER_PRINT_DEBUG
    cout << "File loaded: " << endl;
    for (auto i = 0; i < vertices->size(); i++) {
        cout << "vertex: " << vertices->at(i)->toStr() << endl;
    }
#endif
}


ObjLoader::~ObjLoader()
{
    for (Vec* v : *vertices) {
        delete v;
    }
    vertices->clear();
    faceIndexes->clear();
    verticesIndexes->clear();
}
