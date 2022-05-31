#include "Trans.h"
#include "Primitive.h"


namespace CatmullRom {
    void buildRotMatrix(float* x, float* y, float* z, float* m);
    void cross(float* a, float* b, float* res);
    void normalize(float* a);
    float length(float* v);
    void multMatrixVector(float* m, float* v, float* res);
    void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float* pos, float* deriv);
    void getGlobalCatmullRomPoint(std::vector<Point> catmullPoints, float gt, float* pos, float* deriv);
}