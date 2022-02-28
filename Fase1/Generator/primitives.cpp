#define _USE_MATH_DEFINES
#include <cmath>

#include "primitives.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

void generator::primitive::addVertices(point a, point b, point c){
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
}

void generator::point::definePoint(float lx, float ly, float lz){
    x = lx; y = ly; z = lz;
}

/** como é construído no plano XZ, então y = 0 para qualquer ponto:
 >>> float lx -- comprimento lado x
 >>> float lz -- comprimento lado z
*/
generator::primitive generator::drawPlane(float lx,float lz){

    primitive plane;
    point vert1, vert2, vert3;
    float x = lx/2, z = lz/2;

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