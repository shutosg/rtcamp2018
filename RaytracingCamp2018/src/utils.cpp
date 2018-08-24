#include "utils.h"

vector<string> util::split(const string &str, char sep)
{
    vector<string> v;
    // 見つからない場合は要素数1で返す
    if (str.find(sep) == string::npos) {
        v.push_back(str);
        return v;
    }
    stringstream ss(str);
    string tmp;
    while (getline(ss, tmp, sep)) {
        v.push_back(tmp);
    }
    return v;
}