

#ifndef FASE1_PRIMITIVES_H
#define FASE1_PRIMITIVES_H

#include <vector>
#include <string>

using namespace std;

namespace generator{

    bool parseInput(string, vector<string>);
    bool generatePlane(vector<string>);
    bool generateCone(vector<string>);
    bool generateBox(vector<string>);
    bool generateSphere(vector<string>);
}

#endif //FASE1_PRIMITIVES_H