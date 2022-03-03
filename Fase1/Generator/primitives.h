

#ifndef FASE1_PRIMITIVES_H
#define FASE1_PRIMITIVES_H

#include <vector>
#include <string>

using namespace std;

namespace generator{
/*
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
*/
    bool parseInput(string, vector<string>);
    bool generatePlane(vector<string>);
    bool generateCone(vector<string>);
    bool generateBox(vector<string>);
    bool generateSphere(vector<string>);
}

#endif //FASE1_PRIMITIVES_H