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
    point vert1;
    point vert2;
    point vert3;
    float x = lx/2;
    float z = lz/2;

    // quadrado inferior x > 0 && z > 0
    vert1.definePoint(x,0,z);
    vert2.definePoint(x,0,0);
    vert3.definePoint(0,0,0);
    plane.addVertices(vert1,vert2,vert3);

    vert1.definePoint(0,0,0);
    vert2.definePoint(0,0,z);
    vert3.definePoint(x,0,z);
    plane.addVertices(vert1,vert2,vert3);

    // quadrado inferior x < 0 && z > 0
    vert1.definePoint(0,0,z);
    vert2.definePoint(0,0,0);
    vert3.definePoint(-x,0,0);
    plane.addVertices(vert1,vert2,vert3);

    vert1.definePoint(-x,0,0);
    vert2.definePoint(-x,0,z);
    vert3.definePoint(0,0,z);
    plane.addVertices(vert1,vert2,vert3);

    // quadrado superior x > 0 && z < 0
    vert1.definePoint(x,0,0);
    vert2.definePoint(x,0,-z);
    vert3.definePoint(0,0,-z);
    plane.addVertices(vert1,vert2,vert3);

    vert1.definePoint(0,0,-z);
    vert2.definePoint(0,0,0);
    vert3.definePoint(x,0,0);
    plane.addVertices(vert1,vert2,vert3);

    // quadrado superior x < 0 && z < 0
    vert1.definePoint(0,0,0);
    vert2.definePoint(0,0,-z);
    vert3.definePoint(-x,0,-z);
    plane.addVertices(vert1,vert2,vert3);

    vert1.definePoint(-x,0,-z);
    vert2.definePoint(-x,0,0);
    vert3.definePoint(0,0,0);
    plane.addVertices(vert1,vert2,vert3);

    return plane;
}