#include <vector>

#ifndef FASE1_PRIMITIVES_H
#define FASE1_PRIMITIVES_H

#endif //FASE1_PRIMITIVES_H

namespace generator{

    class point{
        public:
            float x, y, z;
            void definePoint(float, float, float);
    };

    class primitive{
        public:
            std::vector<point> vertices;
            void addVertices(point, point, point);
    };

    primitive drawPlane(float, float);
}