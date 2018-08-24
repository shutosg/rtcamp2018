#pragma once

// #define OBJLOADER_PRINT_DEBUG

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Vec.hpp"
#include "utils.h"

using namespace std;

class ObjLoader
{
public:
    ObjLoader();
    ObjLoader(const string &fileName);
    ~ObjLoader();
    vector<Vec*> *vertices;
    vector<int> *faceIndexes;
    vector<int> *verticesIndexes;
};

