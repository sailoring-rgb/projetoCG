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
    string res = to_string(6 * (int)(pow(divisions, 2))) + "\n";

    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
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

    float step = length / divisions;
    float deviation = length / 2;


    // string que guarda os pontos que estruturam a figura
    string res = to_string(6 * (int)(pow(divisions, 2) * 6)) + "\n";

    for (int j = 0; j < divisions; j++) {
        for (int i = 0; i < divisions; i++) {
            // DOWN FACE
            p1 = to_string(i * step - deviation) + "," + to_string(-deviation) + "," + to_string(j * step - deviation) + "\n";
            p2 = to_string(i * step - deviation) + "," + to_string(-deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p3 = to_string((i + 1) * step - deviation) + "," + to_string(-deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(-deviation) + "," + to_string(j * step - deviation) + "\n";
            res = res + p1 + p3 + p2 + p3 + p1 + p4;

            // RIGHT FACE
            p1 = to_string(length - deviation) + "," + to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "\n";
            p2 = to_string(length - deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p3 = to_string(length - deviation) + "," + to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p4 = to_string(length - deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "\n";
            res = res + p1 + p2 + p3 + p4 + p2 + p1;

            // UP FACE
            p1 = to_string(i * step - deviation) + "," + to_string(length - deviation) + "," + to_string(j * step - deviation) + "\n";
            p2 = to_string(i * step - deviation) + "," + to_string(length - deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p3 = to_string((i + 1) * step - deviation) + "," + to_string(length - deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(length - deviation) + "," + to_string(j * step - deviation) + "\n";
            res = res + p1 + p2 + p3 + p4 + p1 + p3;

            // LEFT FACE
            p1 = to_string(-deviation) + "," + to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p2 = to_string(-deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "\n";
            p3 = to_string(-deviation) + "," + to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "\n";
            p4 = to_string(-deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "\n";
            res = res + p1 + p2 + p3 + p2 + p4 + p3;

            // FRONT FACE
            p1 = to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(length - deviation) + "\n";
            p2 = to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(length - deviation) + "\n";
            p3 = to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(length - deviation) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(length - deviation) + "\n";
            res = res + p1 + p2 + p3 + p4 + p1 + p3;

            // BACK FACE
            p1 = to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(-deviation) + "\n";
            p2 = to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(-deviation) + "\n";
            p3 = to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(-deviation) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(-deviation) + "\n";
            res = res + p1 + p2 + p3 + p3 + p2 + p4;
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

    if (radius < 0 || height < 0 || slices < 0 || stack < 0) {
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
            float p4y = HeightofStack * (i + 1);
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

bool generateCylinder(vector<string> params) {
    // argumentos: float radius, float height, int slices, file.3d

    float radius = stof(params[0]);
    float height = stof(params[1]);
    int slices = stoi(params[2]);

    if (radius < 0 || height < 0 || slices < 0) return false;

    string file = params[3];
    int found = file.find(".3d");
    if (found <= 0) return false;

    float step = (2 * M_PI) / slices;
    float x1, x2, z1, z2, upper_h = height / 2, lower_h = -upper_h;
    float alpha, next_alpha;
    string aux = "";
    int totalPoints = 0;

    for (int i = 0; i < slices; i++) {
        alpha = step * i;
        next_alpha = alpha + step;
        x1 = radius * sin(alpha);
        z1 = radius * cos(alpha);
        x2 = radius * sin(next_alpha);
        z2 = radius * cos(next_alpha);

        string p0 = to_string(0) + "," + to_string(upper_h) + "," + to_string(0) + "\n";
        string p1 = to_string(x1) + "," + to_string(upper_h) + "," + to_string(z1) + "\n";
        string p2 = to_string(x2) + "," + to_string(upper_h) + "," + to_string(z2) + "\n";
        string p3 = to_string(0) + "," + to_string(lower_h) + "," + to_string(0) + "\n";
        string p4 = to_string(x1) + "," + to_string(lower_h) + "," + to_string(z1) + "\n";
        string p5 = to_string(x2) + "," + to_string(lower_h) + "," + to_string(z2) + "\n";
        aux += p2 + p0 + p1 + p4 + p3 + p5 + p1 + p4 + p5 + p1 + p5 + p2;

        totalPoints += 12;
    }
    string res = to_string(totalPoints) + "\n" + aux;
    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}


bool generateSphere(vector<string> params) {
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
    string aux = "";

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
    for (int i = 0; i < totalSlices; i++) {
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

        }
    }
    string res = to_string(totalPoints) + "\n" + aux;
    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}

bool generateTorus(vector<string>params) {
    float innerR = stof(params[0]);
    float outterR = stof(params[1]);
    int slices = stoi(params[2]);
    int stacks = stoi(params[3]);

    if (innerR < 0 || outterR < 0 || slices < 0 || stacks < 0)
        return false;

    string file = params[4];
    int found = file.find(".3d");
    if (found <= 0) return false;

    string aux = "";

    double rad = (innerR + outterR) / 2;
    double dist = rad - innerR;

    float stepSlice = (2 * M_PI) / slices;
    float stepStack = (2 * M_PI) / stacks;

    int totalPoints = 0;
    float x, y, z;

    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            x = (rad + dist * cos(stepSlice * i)) * cos(stepStack * j);
            y = dist * sin(stepSlice * i);
            z = (rad + dist * cos(stepSlice * i)) * sin(stepStack * j);
            string p1 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "\n";

            x = (rad + dist * cos(stepSlice * (i + 1))) * cos(stepStack * j);
            y = dist * sin(stepSlice * (i + 1));
            z = (rad + dist * cos(stepSlice * (i + 1))) * sin(stepStack * j);
            string p2 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "\n";

            x = (rad + dist * cos(stepSlice * (i + 1))) * cos(stepStack * (j + 1));
            y = dist * sin(stepSlice * (i + 1));
            z = (rad + dist * cos(stepSlice * (i + 1))) * sin(stepStack * (j + 1));
            string p3 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "\n";

            x = (rad + dist * cos(stepSlice * i)) * cos(stepStack * (j + 1));
            y = dist * sin(stepSlice * i);
            z = (rad + dist * cos(stepSlice * i)) * sin(stepStack * (j + 1));
            string p4 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "\n";

            aux = aux + p1 + p2 + p4 + p2 + p3 + p4;
            totalPoints += 6;
        }
    }

    string res = to_string(totalPoints) + "\n" + aux;
    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}

bool generateHalfSphere(vector<string> params) {
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

    // String para guardar os pontos usados na construção da semiesfera
    string aux = "";

    // Definição dos limites da esfera em radianos
    double startSlice = 0;
    double startStack = 0;
    double endSlice = M_PI;
    double endStack = M_PI;

    // Definição dos passos dados em casa slice e stack
    double stepSlice = (endSlice - startSlice) / totalSlices;
    double stepStack = (endStack - startStack) / totalStacks;

    // Pontos
    double x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3;

    // Ciclo para determinar os pontos da seiesfermi
    for (int i = 0; i < totalSlices; i++) {
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

        }
    }

    for(int i = 0 ; i < 30; i++){
        float theta = 2 * M_PI / 30;
        float x = radius * cosf(theta * i);
        float y = radius * sinf(theta * i);
        float x2 = radius * cosf(theta * (i + 1));
        float y2 = radius * sinf(theta * (i + 1));

        string p0 = to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
        string p1 = to_string(x) + "," + to_string(y) + "," + to_string(0) + "\n";
        string p2 = to_string(x2) + "," + to_string(y2) + "," + to_string(0) + "\n";

        aux = aux + p1 + p0 + p2;
        totalPoints += 3;
    }

    string res = to_string(totalPoints) + "\n" + aux;
    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}

bool generateEllipsoid(vector<string> params){

    double radius = stod(params[0]);
    double height = stod(params[1]);
    int totalSlices = stoi(params[2]);
    int totalStacks = stoi(params[3]);

    // Validação dos parâmetros recebidos
    if (radius < 0 || height < 0 || totalSlices < 0 || totalStacks < 0)
        return false;

    // Validação da existência do ficheiro
    string file = params[4];
    int found = file.find(".3d");
    if (found <= 0) return false;

    int totalPoints = 0;

    // String para guardar os pontos usados na construção do elipsóide
    string aux ="";

    // Definição dos limites do elipsóide
    double startSlice = 0;
    double startStack = 0;
    double endSlice = M_PI * 2;
    double endStack = M_PI;

    // Definição dos passos dados em casa slice e stack
    double stepSlice = (endSlice - startSlice) / totalSlices;
    double stepStack = (endStack - startStack) / totalStacks;

    // Pontos
    double x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3;

    // Ciclo para determinar os pontos do elipsóide
    for(int i = 0; i < totalSlices; i++) {
        for (int j = 0; j < totalStacks; j++) {
            double u = i * stepSlice + startSlice;
            double v = j * stepStack + startStack;
            double un = (i + 1 == totalSlices) ? endSlice : (i + 1) * stepSlice + startSlice;
            double vn = (j + 1 == totalStacks) ? endStack : (j + 1) * stepStack + startStack;

            x0 = cos(u) * sin(v) * radius;
            y0 = cos(v) * height;
            z0 = sin(u) * sin(v) * radius;
            x1 = cos(u) * sin(vn) * radius;
            y1 = cos(vn) * height;
            z1 = sin(u) * sin(vn) * radius;
            x2 = cos(un) * sin(v) * radius;
            y2 = cos(v) * height;
            z2 = sin(un) * sin(v) * radius;
            x3 = cos(un) * sin(vn) * radius;
            y3 = cos(vn) * height;
            z3 = sin(un) * sin(vn) * radius;

            totalPoints += 6;

            string p0 = to_string(x0) + "," + to_string(y0) + "," + to_string(z0) + "\n";
            string p1 = to_string(x1) + "," + to_string(y1) + "," + to_string(z1) + "\n";
            string p2 = to_string(x2) + "," + to_string(y2) + "," + to_string(z2) + "\n";
            string p3 = to_string(x3) + "," + to_string(y3) + "," + to_string(z3) + "\n";
            aux = aux + p3 + p1 + p2 + p2 + p1 + p0;

        }
    }
    string res = to_string(totalPoints) + "\n" + aux;
    writeInFile(res,file);
    printf("File gerado com sucesso");
    return true;
}

bool generatePatch(vector<string> params){
    // PARAMS:
    // [0] name_patch_file
    // [1] tessellation_lvl
    double tessellation_lvl = stod(params[1]);
    string patch;

    if (tessellation_lvl < 0)
        return false;

    ifstream file_patch;

    file_patch.open("../../Models/"+params[0]);

    if(!file_patch.is_open()){
        printf("Cannot open file.\n");
        return false;
    }
    string nr;
    getline(file_patch,nr);
    int nrPatches = stoi(nr);

    string aux;

    for(int i = 0 ; i < nrPatches; i++){
        // In each cycle iteration aux has the patch #i
        getline(file_patch,aux);
    }

    file_patch.close();





    // CALCULATE THE POINTS

    // WRITE ON AUX
/*
    string output = params[0];
    int out_found = output.find(".3d");
    if (out_found <= 0) return false;

    string aux = NULL;
    int totalPoints = 0;
    string res = to_string(totalPoints) + "\n" + aux;

    writeInFile(res,output);
    printf("File gerado com sucesso");
*/

    return true;
}


bool parseInput(string primitive, vector<string> params) {
    int option = -1;

    if (primitive.compare("box") == 0) option = 1;
    else if (primitive.compare("cone") == 0) option = 2;
    else if (primitive.compare("plane") == 0) option = 3;
    else if (primitive.compare("sphere") == 0) option = 4;
    else if (primitive.compare("cylinder") == 0) option = 5;
    else if (primitive.compare("torus") == 0) option = 6;
    else if (primitive.compare("asteroids") == 0) option = 4;
    else if (primitive.compare("ellipsoid") == 0) option = 7;
    else if (primitive.compare("hsphere") == 0) option = 8;
    else if (primitive.compare("patch") == 0) option = 9;

    bool ret = false;

    switch (option) {
    case 1:
        if (params.size() == 3 || params.size() == 4) {
            ret = generateBox(params);
        }
        else ret = false;
        break;
    case 2:
        if (params.size() == 5) {
            ret = generateCone(params);
        }
        else ret = false;
        break;
    case 3:
        if (params.size() == 3) {
            ret = generatePlane(params);
        }
        else ret = false;
        break;
    case 4:
        if (params.size() == 4) {
            ret = generateSphere(params);
        }
        else ret = false;
        break;
    case 5:
        if (params.size() == 4) {
            ret = generateCylinder(params);
        }
        else ret = false;
        break;
    case 6:
        if (params.size() == 5) {
            ret = generateTorus(params);
        }
        else ret = false;
        break;
    case 7:
        if (params.size() == 5) {
            ret = generateEllipsoid(params); 
        }
        else ret = false;
        break;
    case 8:
        if (params.size() == 4) {
            ret = generateHalfSphere(params); 
        }
        else ret = false;
        break;
    case 9:
        if (params.size() == 2) {
            ret = generatePatch(params);
        }
        else ret = false;
        break;
    default:
        break;
    }

    return ret;
}

/**
* Function that Iniciates the Generator
*/
int main(int argc, char** argv) {
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
