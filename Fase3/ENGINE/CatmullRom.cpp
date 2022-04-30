#include <cmath>
#include <iterator>
#include <string>

#include "CatmullRom.h"
#include "Point.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include <GL/glew.h>
#include <GL/glut.h>
#else
#endif

int pointcount = 1;

void CatmullRom::buildRotMatrix(float* x, float* y, float* z, float* m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void CatmullRom::cross(float* a, float* b, float* res) {

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void CatmullRom::normalize(float* a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}

float CatmullRom::length(float* v) {

    float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return res;

}

void CatmullRom::multMatrixVector(float* m, float* v, float* res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];;
        }
    }

}

void CatmullRom::getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float* pos, float* deriv) {

    // catmull-rom matrix
    float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                        { 1.0f, -2.5f,  2.0f, -0.5f},
                        {-0.5f,  0.0f,  0.5f,  0.0f},
                        { 0.0f,  1.0f,  0.0f,  0.0f} };


    // Compute pos = T * A
    float a[4];

    float p[4] = { p0.getX(), p1.getX(), p2.getX(), p3.getX() };
    multMatrixVector((float*)m, p, a);
    pos[0] = t*t*t * a[0] + t*t * a[1] + t * a[2] + a[3];

    float p_1[4] = { p0.getY(), p1.getY(), p2.getY(), p3.getY() };
    multMatrixVector((float*)m, p_1, a);
    pos[1] = t*t*t * a[0] + t*t * a[1] + t * a[2] + a[3];

    float p_2[4] = { p0.getZ(), p1.getZ(), p2.getZ(), p3.getZ() };
    multMatrixVector((float*)m, p_2, a);
    pos[2] = t*t*t * a[0] + t*t * a[1] + t * a[2] + a[3];

    // compute deriv = T' * A
    float a1[4];

    float pp[4] = { p0.getX(), p1.getX(), p2.getX(), p3.getX() };
    multMatrixVector((float*)m, pp, a1);
    deriv[0] = 3*t*t * a1[0] + 2*t * a1[1] + a1[2];


    float pp1[4] = { p0.getY(), p1.getY(), p2.getY(), p3.getY() };
    multMatrixVector((float*)m, pp1, a1);
    deriv[1] = 3*t*t * a1[0] + 2*t * a1[1] + a1[2];


    float pp2[4] = { p0.getZ(), p1.getZ(), p2.getZ(), p3.getZ() };
    multMatrixVector((float*)m, pp2, a1);
    deriv[2] = 3*t*t * a1[0] + 2*t * a1[1] + a1[2];
}


// given  global t, returns the point in the curve
void CatmullRom::getGlobalCatmullRomPoint(std::vector<Point> catmullPoints, float gt, float* pos, float* deriv) {
    
    pointcount = catmullPoints.size();

    float t = gt * pointcount; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + pointcount - 1) % pointcount;
    indices[1] = (indices[0] + 1) % pointcount;
    indices[2] = (indices[1] + 1) % pointcount;
    indices[3] = (indices[2] + 1) % pointcount;

    getCatmullRomPoint(t, catmullPoints[indices[0]], catmullPoints[indices[1]], catmullPoints[indices[2]], catmullPoints[indices[3]], pos, deriv);
}


void CatmullRom::renderCatmullRomCurve(std::vector<Point> catmullPoints) {

    float pos[3];
    float deriv[3];

    // draw curve using line segments with GL_LINE_LOOP
    glBegin(GL_LINE_LOOP);
    float i = 0;
    while (i < 100.0f) {
        getGlobalCatmullRomPoint(catmullPoints, i / 100.0f, pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();

    glBegin(GL_LINES);
    for (int i = 0; i < 100; i += 1) {
        getGlobalCatmullRomPoint(catmullPoints, i / 100.0f, pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);
        pos[0] += deriv[0];
        pos[1] += deriv[1];
        pos[2] += deriv[2];
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();
}



  