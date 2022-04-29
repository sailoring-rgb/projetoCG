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
#include <sstream>

using namespace tinyxml2;
using namespace std;

//------------------
class Point {
    public:

	float x;
	float y;
	float z;

	Point() {
		x = 0;
		y = 0;
		z = 0;
	}

	Point(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}


	float getX() {
		return x;
	}

	float getY() {
		return y;
	}

	float getZ() {
		return z;
	}

	void setX(float x) {
		this->x = x;
	}

	void setY(float y) {
		this->y = y;
	}


	void setZ(float z) {
		this->z = z;
	}
};
//-----------------

// Função para escrever num ficheiro
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

// Função para gerar os pontos do Plano
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

// Função para gerar os pontos de um cubo
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


// Função para gerar os pontos do Cone
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

// Função para gerar os pontos do Cilindro
bool generateCylinder(vector<string> params) {
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

// Função para gerar os pontos da esfera
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

// Função para gerar os pontos do Toro
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

// Função para gerar os pontos da semiesfera
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

// Função para gerar os pontos do elipsóide
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

// Partir uma string segundo um delimitador
vector<string> splitter(string s, string del) {
    vector<string> res;
    int start = 0;
    int end = s.find(del);

    while (end != -1) {
        res.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    res.push_back(s.substr(start, end - start));
    return res;
}

// Multiplicação: matrix_float * matrix_point
vector<vector<Point>> multMatrixPoints(vector<vector<float>> m, vector<vector<Point>> patch) {
    int i, j, k;
    float x, y, z;
    Point p, pm;
    vector<vector<Point>> res(4,vector<Point>(4));
    
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            res[i][j] = Point(0, 0, 0);

    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j){
            for (k = 0; k < 4; ++k) {
                p = patch[k][j];
                x = p.getX();
                y = p.getY();
                z = p.getZ();
                p = Point(x * m[i][k], y * m[i][k], z * m[i][k]);
                pm = res[i][j];
                res[i][j] = Point(pm.getX() + p.getX(), pm.getY() + p.getY(), pm.getZ() + p.getZ());
            }
        }
    }
    return res;
}

// Multiplicação: matrix_point * matrix_float
vector<vector<Point>> multPointsMatrix(vector<vector<Point>> mat, vector<vector<float>> m) {
    int i, j, k;
    float x, y, z;
    Point p, pm;
    vector<vector<Point>> res(4,vector<Point>(4));

    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            res[i][j] = Point(0, 0, 0);    

    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j){
            for (k = 0; k < 4; ++k) {
                p = mat[i][k];
                x = p.getX();
                y = p.getY();
                z = p.getZ();
                p = Point(x * m[k][j], y * m[k][j], z * m[k][j]);
                pm = res[i][j];
                res[i][j] = Point(pm.getX() + p.getX(), pm.getY() + p.getY(), pm.getZ() + p.getZ());
            }
        }
    }
    return res;
}

// Multiplicação: vetor * matrix_points
vector<Point> multVectorMatrix(vector<float> vect, vector<vector<Point>> matrix) {
    int j, k;
    float x, y, z;
    Point p, pm;
    vector<Point> res(4);

    for (j = 0; j < 4; ++j)
        res[j] = Point(0, 0, 0);

    for (j = 0; j < 4; ++j) {
        for (k = 0; k < 4; ++k) {
            p = matrix[k][j];
            x = p.getX();
            y = p.getY();
            z = p.getZ();
            p = Point(x * vect[k], y * vect[k], z * vect[k]);
            pm = res[j];
            res[j] = Point(pm.getX() + p.getX(), pm.getY() + p.getY(), pm.getZ() + p.getZ());
        }
    }
    return  res;
}

// Multiplicação: vetor * matrix_points
Point multVectors(vector<Point> vector, ::vector<float> vect) {
    Point p = Point(0, 0, 0);
    int j;

    for(j = 0; j < 4; ++j){
        p.setX(p.getX() + vector[j].getX() * vect[j]);
        p.setY(p.getY() + vector[j].getY() * vect[j]);
        p.setZ(p.getZ() + vector[j].getZ() * vect[j]);
    }
    return p;
}

// Funcção para gerar os vetores u e v 
vector<float> vectorGenerator(float f) {
    vector<float> vect(4);

    vect[0] = pow(f,3);
    vect[1] = pow(f,2);
    vect[2] = f;
    vect[3] = 1;

    return vect;
}

// Função para criação dos pontos dos triângulos de um patch
vector<Point> triangulacao(vector<vector<vector<Point>>> patches_set, int tessellation_lvl) {
    int i, j, k;
    float pu = 0.0f, pv = 0.0f;
    vector<float> u, v;
    vector<Point> triangles, res;
    vector<vector<Point>> aux, matrix;
    vector<vector<Point>> grid(tessellation_lvl + 1, vector<Point> (tessellation_lvl + 1));
    Point final;
    
    vector<vector<float>> m
    {
        {-1.0f,3.0f,-3.0f,1.0f},
        {3.0f,-6.0f,3.0f,0.0f},
        {-3.0f,3.0f,0.0f,0.0f},
        {1.0f,0.0f,0.0f,0.0f},
    };

    for (j = 0; j <= tessellation_lvl; j++)
        for (k = 0; k <= tessellation_lvl; k++)
            grid[j][k] = Point(0, 0, 0);

    for(i = 0; i< patches_set.size(); i++) {
        aux = multMatrixPoints(m, patches_set[i]);
        matrix = multPointsMatrix(aux, m);

        for (int j = 0; j <= tessellation_lvl; j++) {
            pv = ((float) j) / ((float) tessellation_lvl);
            v = vectorGenerator(pv);
            for (int k = 0; k <= tessellation_lvl; k++) {
                pu = ((float) k) / ((float) tessellation_lvl);
                u = vectorGenerator(pu);
                res = multVectorMatrix(u, matrix);
                final = multVectors(res, v);
                grid[j][k] = Point(final.getX(), final.getY(), final.getZ());
            }
        }

        for (j = 0; j < tessellation_lvl; j++) {
            for (k = 0; k < tessellation_lvl; k++) {
                triangles.push_back(grid[j][k]);
                triangles.push_back(grid[j+1][k]);
                triangles.push_back(grid[j][k+1]);
                triangles.push_back(grid[j+1][k]);
                triangles.push_back(grid[j+1][k+1]);
                triangles.push_back(grid[j][k+1]);
            }
        }
    }
    return triangles;
}

// Função para processar um patch
bool generatePatch(vector<string> params){
    ifstream file_patch;

    file_patch.open("../../Patches/"+params[0]);

    if(!file_patch.is_open()){
        printf("Cannot open file.\n");
        return false;
    }

    string line, res;
    int i = 0, n, totalPatches, totalControlPoints = 0, count, index, tessellation = stoi(params[1]);
    Point p;
    vector<string> all_patches;
    vector<Point> controlPoints, coordenadas;
    vector<vector<vector<Point>>> patches;

    while (getline(file_patch, line)) {
        if (i == 0)
            totalPatches = stoi(line);
        else if (i <= totalPatches)
            all_patches.push_back(line);
        else if (i == totalPatches + 1)
            totalControlPoints = stoi(line);
        else if(i > totalPatches + 1) {
            line = line.substr(line.find(" ") + 1);
            vector<string> res = splitter(line, ", ");
            Point p = Point(stof(res[0]), stof(res[1]), stof(res[2]));
            controlPoints.push_back(p);
        }
        i++;
    }

    for (n = 0; n < totalPatches; n++) {
        count = 0;
        vector<Point> curve(4);
        vector<vector<Point>> single_patch;
        vector<int> indices;
        vector<string> res;
        
        res = splitter(all_patches[n], ", ");
        
        for (i = 0; i < 16; i++) 
            indices.push_back(stoi(res[i]));

        for (i = 0; i < indices.size(); i++) {
            index = indices[i];
            p = controlPoints[index];
            if(count<4){
                curve[count] = p;
                if(i == indices.size() - 1)
                    single_patch.push_back(curve);
            }
            else{
                count = 0;
                single_patch.push_back(curve);
                curve[count] = p;
            }
            count++;
        }
        patches.push_back(single_patch);
    }

    coordenadas = triangulacao(patches,tessellation);

    res = to_string(coordenadas.size())+"\n";

    for(i = 0; i < coordenadas.size(); i++){
        p = coordenadas[i];
        res += to_string(p.getX()) + ", " + to_string(p.getY()) + ", "+ to_string(p.getZ()) + "\n";
    }

    string output_file = params[0];
    output_file.erase(remove(output_file.begin(), output_file.end(), '.'), output_file.end());
    output_file += to_string(tessellation) + ".3d";

    writeInFile(res, output_file);

    return true;
}

// Função para processar o input recebido na main
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

// Função para iniciar o gerador
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
