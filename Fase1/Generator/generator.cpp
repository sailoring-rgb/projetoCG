#define _USE_MATH_DEFINES

#ifdef __APPLE__
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <io.h>
#else
#endif

#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include <cstring>

using namespace tinyxml2;
using namespace std;

void writeInFile(string res, string file) {
    //generats XML file using tinyxml2
    char tmp[256];

    if (getcwd(tmp, 256) != NULL) { //tmp which contains the directory

        string path(tmp);

        int found = path.find("Generator"); // finds generator's localization

        replace(path.begin(), path.end(), '\\', '/');
        path.erase(path.begin() + found, path.end());
        string pathXML = path + "Models/model.xml";
        strcpy(tmp, pathXML.c_str());
        string path3D = path + "Models/" + file;
        ofstream File3D(path3D);
        File3D << res;
        File3D.close();

        const char* c = file.c_str();

        XMLDocument doc;
        doc.LoadFile(tmp);
        XMLNode* pRoot = doc.FirstChild();

        XMLElement* pElement = doc.NewElement("model");
        pElement->SetAttribute("file", c);

        pRoot->InsertEndChild(pElement);

        doc.SaveFile(tmp);
    }
}

/*
    Fun��o para gerar os pontos do Plano
*/


bool generatePlane(vector<string> params) {

    float length = stof(params[0]);
    float divisions = stof(params[1]);
    if (length < 0 || divisions < 0) return false;

    string file = params[2];
    int found = file.find(".3d");
    if (found <= 0) return false;

    string p1, p2, p3, p4;
    float x = length / divisions, z = length / divisions;

    // string que guarda os pontos que estruturam a figura
    string res = to_string(6 * (int)(pow(divisions,2))) + "\n";

    for (double row = -divisions/2.0; row < divisions/2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions/2.0; column > -divisions/2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(x * column) + "," + to_string(0.000000) + "," + to_string(z * nextRow) + "\n";
            p2 = to_string(x * column) + "," + to_string(0.000000) + "," + to_string(z * row) + "\n";
            p3 = to_string(x * (nextColumn)) + "," + to_string(0.000000) + "," + to_string(z * row) + "\n";
            p4 = to_string(x * nextColumn) + "," + to_string(0.000000) + "," + to_string(z * nextRow) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }

    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}

bool generateBox(vector<string> params) {

    float length = stof(params[0]);
    float divisions = stof(params[1]);
    if (length < 0 || divisions < 0) return false;

    string file = params[2];
    int found = file.find(".3d");
    if (found <= 0) return false;

    string p1, p2, p3, p4;

    float x = length / divisions, y = length / divisions, z = length / divisions;

    // string que guarda os pontos que estruturam a figura
    string res = to_string(6 * (int)(pow(divisions, 2) * 6)) + "\n";

    //Plano XZ
    //y=-length/2.0
    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(x * column) + "," + to_string(-length / 2.0) + "," + to_string(z * nextRow) + "\n";
            p2 = to_string(x * column) + "," + to_string(-length / 2.0) + "," + to_string(z * row) + "\n";
            p3 = to_string(x * nextColumn) + "," + to_string(-length / 2.0) + "," + to_string(z * row) + "\n";
            p4 = to_string(x * nextColumn) + "," + to_string(-length / 2.0) + "," + to_string(z * nextRow) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }

    //y=length/2.0
    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(x * column) + "," + to_string(length / 2.0) + "," + to_string(z * nextRow) + "\n";
            p2 = to_string(x * column) + "," + to_string(length / 2.0) + "," + to_string(z * row) + "\n";
            p3 = to_string(x * nextColumn) + "," + to_string(length / 2.0) + "," + to_string(z * row) + "\n";
            p4 = to_string(x * nextColumn) + "," + to_string(length / 2.0) + "," + to_string(z * nextRow) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }

    //Plano XY
    //z=length/2
    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(x * column) + "," + to_string(y * nextRow) + "," + to_string(length/2.0) + "\n";
            p2 = to_string(x * column) + "," + to_string(y * row) + "," + to_string(length / 2.0) + "\n";
            p3 = to_string(x * nextColumn) + "," + to_string(y * row) + "," + to_string(length / 2.0) + "\n";
            p4 = to_string(x * nextColumn) + "," + to_string(y * nextRow) + "," + to_string(length / 2.0) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }

    //z=-length/2
    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(x * column) + "," + to_string(y * nextRow) + "," + to_string(-length / 2.0) + "\n";
            p2 = to_string(x * column) + "," + to_string(y * row) + "," + to_string(-length / 2.0) + "\n";
            p3 = to_string(x * nextColumn) + "," + to_string(y * row) + "," + to_string(-length / 2.0) + "\n";
            p4 = to_string(x * nextColumn) + "," + to_string(y * nextRow) + "," + to_string(-length / 2.0) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }
    
    //Plano ZY
    //x=length/2.0
    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(length / 2.0) + "," + to_string(y * nextRow) + "," + to_string(z * column) + "\n";
            p2 = to_string(length / 2.0) + "," + to_string(y * row) + "," + to_string(z * column) + "\n";
            p3 = to_string(length / 2.0) + "," + to_string(y * row) + "," + to_string(z * nextColumn) + "\n";
            p4 = to_string(length / 2.0) + "," + to_string(y * nextRow) + "," + to_string(z * nextColumn) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }

    //x=-length/2.0
    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(-length / 2.0) + "," + to_string(y * nextRow) + "," + to_string(z * column) + "\n";
            p2 = to_string(-length / 2.0) + "," + to_string(y * row) + "," + to_string(z * column) + "\n";
            p3 = to_string(-length / 2.0) + "," + to_string(y * row) + "," + to_string(z * nextColumn) + "\n";
            p4 = to_string(-length / 2.0) + "," + to_string(y * nextRow) + "," + to_string(z * nextColumn) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }

    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}


/*
    Fun��o para gerar os pontos do Cone
*/

bool generateCone(vector<string> params) {
    double radius = -1;
    double height = -1;
    int slices = -1;
    int stack = -1;
    radius = stod(params[0]);
    height = stod(params[1]);
    slices = stoi(params[2]);
    stack = stoi(params[3]);


    // String onde s�o guardados o n�mero total de vertices necess�rios para construir o cone
    string res = to_string((2 * slices * stack) * 3) + "\n";

    if (radius < 0 || height < 0 || slices < 0 || stack < 0){
        return false;
    }

    string file = params[4];
    int found = file.find(".3d");
    if (found <= 0) return false;

    //calcular a altura de cada stack
    double HeightofStack = height / stack;

    //Circunferencia de pontos, dados os slices a altura e o raio construido por stack
    for (double i = 0; i < stack; i++) {
        double basestackRadius = ((radius * (height - (HeightofStack * i))) / height);
        double topstackRadius = ((radius * (height - (HeightofStack * (i + 1)))) / height);

        double angle = (2 * M_PI) / slices;

        for (int c = 0; c < slices; c++) {
            double alpha = angle * c;
            double alpha2 = angle * (c + 1);

            float p1x = cos(alpha) * basestackRadius;
            float p1y = HeightofStack * i;
            float p1z = -sin(alpha) * basestackRadius;

            float p2x = cos(alpha2) * basestackRadius;
            float p2y = HeightofStack * i;
            float p2z = -sin(alpha2) * basestackRadius;

            float p3x = cos(alpha) * topstackRadius;
            float p3y = HeightofStack * (i + 1);
            float p3z = -sin(alpha) * topstackRadius;

            float p4x = cos(alpha2) * topstackRadius;
            float p4y = HeightofStack * (i+1);
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
    printf("File gerado com sucesso");
    return true;
}

bool generateSphere(vector<string> params){
    double radius = stod(params[0]);
    int totalSlices = stoi(params[1]);
    int totalStacks = stoi(params[2]);

    // Validação dos parâmetros recebidos
    if (radius < 0 || totalSlices < 0 || totalStacks < 0)
        return false;

    // Validação da existência do ficheiro
    string file = params[3];
    int found = file.find(".3d");
    if (found <= 0) return false;

    int totalPoints = 0;

    // String para guardar os pontos usados na construção da esfera
    string aux ="";

    // Definição dos limites da esfera em radianos
    double startSlice = 0;
    double startStack = 0;
    double endSlice = M_PI * 2;
    double endStack = M_PI;

    // Definição dos passos dados em casa slice e stack
    double stepSlice = (endSlice - startSlice) / totalSlices;
    double stepStack = (endStack - startStack) / totalStacks;

    // Pontos
    double x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3;

    // Ciclo para determinar os pontos da esfera
    for(int i = 0; i < totalSlices; i++) {
        for (int j = 0; j < totalStacks; j++) {
            double u = i * stepSlice + startSlice;
            double v = j * stepStack + startStack;
            double un = (i + 1 == totalSlices) ? endSlice : (i + 1) * stepSlice + startSlice;
            double vn = (j + 1 == totalStacks) ? endStack : (j + 1) * stepStack + startStack;

            x0 = cos(u) * sin(v) * radius;
            y0 = cos(v) * radius;
            z0 = sin(u) * sin(v) * radius;
            x1 = cos(u) * sin(vn) * radius;
            y1 = cos(vn) * radius;
            z1 = sin(u) * sin(vn) * radius;
            x2 = cos(un) * sin(v) * radius;
            y2 = cos(v) * radius;
            z2 = sin(un) * sin(v) * radius;
            x3 = cos(un) * sin(vn) * radius;
            y3 = cos(vn) * radius;
            z3 = sin(un) * sin(vn) * radius;

            totalPoints += 6;

            string p0 = to_string(x0) + "," + to_string(y0) + "," + to_string(z0) + "\n";
            string p1 = to_string(x1) + "," + to_string(y1) + "," + to_string(z1) + "\n";
            string p2 = to_string(x2) + "," + to_string(y2) + "," + to_string(z2) + "\n";
            string p3 = to_string(x3) + "," + to_string(y3) + "," + to_string(z3) + "\n";
            aux = aux + p3 + p1 + p2 + p2 + p1 + p0;

            // p0, p1, p2
            // p3, p1, p2
            // p3, p0, p2
        }
    }
    string res = to_string(totalPoints) + "\n" + aux;
    writeInFile(res,file);
    printf("File gerado com sucesso");
    return true;
}

bool parseInput(string primitive, vector<string> params) {

    bool ret = false;

    if (primitive.compare("box") == 0) {
        if (params.size() == 3 || params.size() == 4) {
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
        if (params.size() == 3) {
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

/**
* Function that Iniciates the Generator
*/
int main(int argc, char **argv){
    if (argc == 1) {
        cout << "Not enough arguments";
        return 1;
    }

    string primitive(argv[1]);
    vector<string> params;
    transform(primitive.begin(), primitive.end(), primitive.begin(), ::tolower);

    for (int i = 2; i < argc; i++) {
        params.push_back(argv[i]);
    }

    if (!parseInput(primitive, params)) {
        cout << "Arguments for are invalid";
    }
    return 1;
}
