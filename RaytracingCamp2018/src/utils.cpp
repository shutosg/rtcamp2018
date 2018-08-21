#include "utils.h"

vector<string> util::split(const string &str, char sep)
{
    vector<string> v;
    stringstream ss(str);
    string tmp;
    while (getline(ss, tmp, sep)) {
        v.push_back(tmp);
    }
    return v;
}