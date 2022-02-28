#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <io.h>
#else
#endif

#include "primitives.h"
#include <iostream>

using namespace generator;

int main(int argc, char* argv[]){
    primitive a;
    a = drawPlane(3,3);
    return 0;
}