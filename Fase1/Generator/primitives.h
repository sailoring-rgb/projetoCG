#include <vector>

#ifndef FASE1_PRIMITIVES_H
#define FASE1_PRIMITIVES_H

#endif //FASE1_PRIMITIVES_H

namespace generator{

    class Point{
        public:
            float x, y, z;
            void definePoint(float lx, float ly, float lz){
                x = lx; y = ly; z = lz;
            }
    };

    struct primitive{
        std::vector<Point> vertices;
    };

    primitive drawPlane(float, float);
}