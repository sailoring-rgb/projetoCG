#define _USE_MATH_DEFINES
#include <cmath>

#include "primitives.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

/** como é construído no plano XZ, então y = 0 para qualquer ponto:
 >>> float lx -- comprimento lado x
 >>> float lz -- comprimento lado z
*/
generator::primitive generator::drawPlane(float lx,float lz){

    primitive plane;
    Point vert;
    float x = lx/2;
    float z = lz/2;

    // quadrado inferior x > 0 && z > 0
    vert.definePoint(x,0,z);
    vert.definePoint(x,0,0);
    vert.definePoint(0,0,0);
    plane.vertices.push_back(vert);

    vert.definePoint(0,0,0);
    vert.definePoint(0,0,z);
    vert.definePoint(x,0,z);
    plane.vertices.push_back(vert);

    // quadrado inferior x < 0 && z > 0
    vert.definePoint(0,0,z);
    vert.definePoint(0,0,0);
    vert.definePoint(-x,0,0);
    plane.vertices.push_back(vert);

    vert.definePoint(-x,0,0);
    vert.definePoint(-x,0,z);
    vert.definePoint(0,0,z);
    plane.vertices.push_back(vert);

    // quadrado superior x > 0 && z < 0
    vert.definePoint(x,0,0);
    vert.definePoint(x,0,-z);
    vert.definePoint(0,0,-z);
    plane.vertices.push_back(vert);

    vert.definePoint(0,0,-z);
    vert.definePoint(0,0,0);
    vert.definePoint(x,0,0);
    plane.vertices.push_back(vert);

    // quadrado superior x < 0 && z < 0
    vert.definePoint(0,0,0);
    vert.definePoint(0,0,-z);
    vert.definePoint(-x,0,-z);
    plane.vertices.push_back(vert);

    vert.definePoint(-x,0,-z);
    vert.definePoint(-x,0,0);
    vert.definePoint(0,0,0);
    plane.vertices.push_back(vert);

    return plane;
}