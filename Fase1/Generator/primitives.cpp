#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>
#include "primitives.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include "generator.cpp"

using namespace std;

bool parseInput(string primitive, vector<string> params) {
    bool ret;

    if (primitive.compare("box") == 0) {
        if (params.size() == 4 || params.size() == 5) {
            ret = generateBox(params);
        }
        else ret = false;
    }
    else if (primitive.compare("cone") == 0) {
        if (params.size() == 5) {
            ret = generateCone(params);
        }
        else ret = false;
    }
    else if (primitive.compare("plane") == 0) {
        if (params.size() == 4) {
            ret = generatePlane(params);
        }
        else ret = false;
    }
    else if (primitive.compare("sphere") == 0) {
        if (params.size() == 4) {
            ret = generateSphere(params);
        }
        else ret = false;
    }
    return ret;
}

/*
void generator::primitive::addVertices(point a, point b, point c){
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
}

void generator::point::definePoint(float lx, float ly, float lz){
    x = lx; y = ly; z = lz;
}

*/
bool generatePlane(vector<string> params){

    string p1, p2, p3;
    if (params[0] < 0 || params[1] < 0) return false;

    float x = stof(params[0])/2, z = stof(params[1])/2;

    string file = params[2];
    int found = file.find(".3d");
    if (found <= 0) return false;

    // string que guarda os pontos que estruturam a figura
    string res = to_string((2 * slices * stack) * 3) + "\n";

    // quadrado inferior x > 0 && z > 0
    p1 = to_string(x) + "," + to_string(0) + "," + to_string(z) + "\n";
    p2 = to_string(x) + "," + to_string(0) + "," + to_string(0) + "\n";
    p3 = to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
    res = res + p1 + p2 + p3;

    p1 = to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
    p2 = to_string(0) + "," + to_string(0) + "," + to_string(z) + "\n";
    p3 = to_string(x) + "," + to_string(0) + "," + to_string(z) + "\n";
    res = res + p1 + p2 + p3;

    // quadrado inferior x < 0 && z > 0
    p1 = to_string(0) + "," + to_string(0) + "," + to_string(z) + "\n";
    p2 = to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
    p3 = to_string(-x) + "," + to_string(0) + "," + to_string(0) + "\n";
    res = res + p1 + p2 + p3;

    p1 = to_string(-x) + "," + to_string(0) + "," + to_string(0) + "\n";
    p2 = to_string(-x) + "," + to_string(0) + "," + to_string(z) + "\n";
    p3 = to_string(0) + "," + to_string(0) + "," + to_string(z) + "\n";
    res = res + p1 + p2 + p3;

    // quadrado superior x > 0 && z < 0
    p1 = to_string(x) + "," + to_string(0) + "," + to_string(0) + "\n";
    p2 = to_string(x) + "," + to_string(0) + "," + to_string(-z) + "\n";
    p3 = to_string(0) + "," + to_string(0) + "," + to_string(-z) + "\n";
    res = res + p1 + p2 + p3;

    p1 = to_string(0) + "," + to_string(0) + "," + to_string(-z) + "\n";
    p2 = to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
    p3 = to_string(x) + "," + to_string(0) + "," + to_string(0) + "\n";
    res = res + p1 + p2 + p3;

    // quadrado superior x < 0 && z < 0
    p1 = to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
    p2 = to_string(0) + "," + to_string(0) + "," + to_string(-z) + "\n";
    p3 = to_string(-x) + "," + to_string(0) + "," + to_string(-z) + "\n";
    res = res + p1 + p2 + p3;

    p1 = to_string(-x) + "," + to_string(0) + "," + to_string(-z) + "\n";
    p2 = to_string(-x) + "," + to_string(0) + "," + to_string(0) + "\n";
    p3 = to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
    res = res + p1 + p2 + p3;

    writeInFile(res, file);
    return true;
}


bool generateCone(vector<string> params) {
    double radius = stod(params[0]);
    double height = stod(params[1]);
    int slices = stoi(params[2]);
    int stack = stoi(params[3]);


    // String onde são guardados o número total de vertices necessários para construir o cone
    string res = to_string((2 * slices * stack) * 3) + "\n";

    if (radius < 0 || height < 0 || slices < 0 || stack < 0) {
        return false;
    }

    string file = params[4];
    int found = file.find(".3d");
    if (found <= 0) return false;

    //calcular a altura de cada stack
    double Height_of_Stack = height / stack;

    //Circunferencia de pontos, dados os slices a altura e o raio construido por stack
    for (double i = 0; i < stack; i++) {
        double basestackRadius = ((radius * (height - (Height_of_Stack * i))) / height);
        double topstackRadius = ((radius * (height - (Height_of_Stack * (i + 1)))) / height);

        double angle = (2 * M_PI) / slices;

        for (int c = 0; c < slices; c++) {
            double alpha = angle * c;
            double alpha2 = angle * (c + 1);

            float p1x = cos(alpha) * basestackRadius;
            float p1y = Height_of_Stack * i;
            float p1z = -sin(alpha) * basestackRadius;

            float p2x = cos(alpha2) * basestackRadius;
            float p2y = Height_of_Stack * i;
            float p2z = -sin(alpha2) * basestackRadius;

            float p3x = cos(alpha) * topstackRadius;
            float p3y = Height_of_Stack * (i + 1);
            float p3z = -sin(alpha) * topstackRadius;

            float p4x = cos(alpha2) * topstackRadius;
            float p4y = Height_of_Stack * i;
            float p4z = -sin(alpha2) * topstackRadius;

            string p1 = to_string(p1x) + "," + to_string(p1y) + "," + to_string(p1z) + "\n";
            string p2 = to_string(p2x) + "," + to_string(p2y) + "," + to_string(p2z) + "\n";
            string p3 = to_string(p3x) + "," + to_string(p3y) + "," + to_string(p3z) + "\n";
            string p4 = to_string(p4x) + "," + to_string(p4y) + "," + to_string(p4z) + "\n";

            if (i == 0) {
                //Base
                string base = "0.000000,0.000000,0.000000\n";
                res = res + p3 + p1 + p2 + p3 + p2 + p4;
                res = res + base + p2 + p1;
            }
            else if (i != (stack - 1)) {
                res = res + p3 + p1 + p2 + p3 + p2 + p4;
            }
            else res = res + p3 + p1 + p2;
        }
    }

    writeInFile(res, file);
    return true;
}