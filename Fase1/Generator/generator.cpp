#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <io.h>
#else
#endif

#include "primitives.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "primitives.cpp"

using namespace generator;
using namespace std;

/**
* Function that Iniciates the Generator
*/
int main(int argc, char* argv[]){
    if (argc == 1) {
        cout << "Not enough arguments";
        return 1;
    }
    
    string primitive(argv[1]);
    vector<string> params;
    transform(primitive.begin(), primitive.end(), primitive.begin(), ::tolower);

    for (int i = 2; i < argc; i++) {
        params.push_back(argv[i]);
    }

    if (!parseInput(primitive, params)) {
        cout << "Arguments for are invalid";
    }
    return 1;
}