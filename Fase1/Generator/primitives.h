#include <vector>

#ifndef FASE1_PRIMITIVES_H
#define FASE1_PRIMITIVES_H

#endif //FASE1_PRIMITIVES_H

namespace generator{

    class point{
        public:
            float x, y, z;
            void definePoint(float lx, float ly, float lz){
                x = lx; y = ly; z = lz;
            }
    };

    class primitive{
        public:
            std::vector<point> vertices;
            void addVertices(point a, point b, point c){
                vertices.push_back(a);
                vertices.push_back(b);
                vertices.push_back(c);
            }
    };

    primitive drawPlane(float, float);
}