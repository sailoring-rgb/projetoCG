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

//------------------ AUX FUNCTIONS AND DECLARATIONS ------------------//
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

// Função para escrever em ficheiro
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

        if (pRoot != NULL) {
            XMLElement* pElement = doc.NewElement("model");
            pElement->SetAttribute("model", c);
            pRoot->InsertEndChild(pElement);
            doc.SaveFile(tmp);
        }
    }
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

// Funcção para gerar os vetores u e v 
vector<float> vectorGeneratorDerivate(float f) {
    vector<float> vect(4);

    vect[0] = 3 * pow(f, 2);
    vect[1] = 2 * f;
    vect[2] = 1;
    vect[3] = 0;

    return vect;
}

void normalize(float* a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    if (l != 0) {
        a[0] = a[0] / l;
        a[1] = a[1] / l;
        a[2] = a[2] / l;
    }
}

// Função para criação dos pontos dos triângulos de um patch
vector<Point> triangulacao(vector<vector<vector<Point>>> patches_set, int tessellation_lvl) {
    int i, j, k;
    float pu = 0.0f, pv = 0.0f;
    vector<float> u, v, du, dv;
    vector<Point> triangles, res, res2;
    vector<vector<Point>> aux, matrix;
    vector<vector<Point>> grid(tessellation_lvl + 1, vector<Point> (tessellation_lvl + 1));
    vector<vector<Point>> gridN(tessellation_lvl + 1, vector<Point>(tessellation_lvl + 1));
    Point final;
    Point normal;
    
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
            dv = vectorGeneratorDerivate(pv);
            for (int k = 0; k <= tessellation_lvl; k++) {
                pu = ((float) k) / ((float) tessellation_lvl);
                u = vectorGenerator(pu);
                du = vectorGeneratorDerivate(pu);
                res = multVectorMatrix(u, matrix);
                final = multVectors(res, v);
                grid[j][k] = Point(final.getX(), final.getY(), final.getZ());

                res2 = multVectorMatrix(du,matrix);
                normal = multVectors(res2, dv);
                gridN[j][k] = Point(normal.getX(), normal.getY(), normal.getZ());
            }
        }

        for (j = 0; j < tessellation_lvl; j++) {
            for (k = 0; k < tessellation_lvl; k++) {
                triangles.push_back(grid[j][k]);
                triangles.push_back(gridN[j][k]);

                triangles.push_back(grid[j+1][k]);
                triangles.push_back(gridN[j + 1][k]);

                triangles.push_back(grid[j][k+1]);
                triangles.push_back(gridN[j][k + 1]);

                triangles.push_back(grid[j+1][k]);
                triangles.push_back(gridN[j + 1][k]);

                triangles.push_back(grid[j+1][k+1]);
                triangles.push_back(gridN[j + 1][k + 1]);

                triangles.push_back(grid[j][k+1]);
                triangles.push_back(gridN[j][k + 1]);
            }
        }
    }
    return triangles;
}

//------------------ FUNCTIONS TO COMPUTE POINTS ------------------//
// Função para gerar os pontos do Plano
bool generatePlane(vector<string> params) {

    float length = stof(params[0]);
    float divisions = stof(params[1]);
    if (length < 0 || divisions < 0) return false;

    string file = params[2];
    int found = file.find(".3d");
    if (found <= 0) return false;

    string p1, p2, p3, p4;
    float pn[3] = {0.0, 1.0, 0.0};
    float x = length / divisions, z = length / divisions;

    // string que guarda os pontos que estruturam a figura
    string res = to_string(6 * (int)(pow(divisions, 2))) + "\n";

    for (double row = -divisions / 2.0; row < divisions / 2.0; row++) {
        double tempR = row;
        double nextRow = tempR + 1;
        for (double column = divisions / 2.0; column > -divisions / 2.0; column--) {
            double tempC = column;
            double nextColumn = tempC - 1;
            p1 = to_string(x * column) + "," + to_string(0.000000) + "," + to_string(z * nextRow) + "," +
                    to_string(pn[0]) + "," + to_string(pn[1]) + "," + to_string(pn[2]) + "," +
                    to_string(1) + "," + to_string(0) + "\n";
            p2 = to_string(x * column) + "," + to_string(0.000000) + "," + to_string(z * row) + "," +
                    to_string(pn[0]) + "," + to_string(pn[1]) + "," + to_string(pn[2]) + "," +
                    to_string(1) + "," + to_string(1) + "\n";
            p3 = to_string(x * (nextColumn)) + "," + to_string(0.000000) + "," + to_string(z * row) + "," +
                    to_string(pn[0]) + "," + to_string(pn[1]) + "," + to_string(pn[2]) + "," +
                    to_string(0) + "," + to_string(1) + "\n";
            p4 = to_string(x * nextColumn) + "," + to_string(0.000000) + "," + to_string(z * nextRow) + "," +
                    to_string(pn[0]) + "," + to_string(pn[1]) + "," + to_string(pn[2]) + "," +
                    to_string(0) + "," + to_string(0) + "\n";
            res = res + p1 + p2 + p3 + p3 + p4 + p1;
        }
    }

    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}

// Função para determinar os pontos do cubo
bool generateBox(vector<string> params) {

    float length = stof(params[0]);
    float divisions = stof(params[1]);
    if (length < 0 || divisions < 0) return false;

    string file = params[2];
    int found = file.find(".3d");
    if (found <= 0) return false;

    string p1, p2, p3, p4;
    float pnd[3] = { 0.0, -1.0, 0.0 }, pnu[3] = { 0.0, 1.0, 0.0 }, pnr[3] = { 1.0, 0.0, 0.0 };
    float pnl[3] = { -1.0, 0.0, 0.0 }, pnf[3] = { 0.0, 0.0, 1.0 }, pnb[3] = { 0.0, 0.0, -1.0 };;

    float step = length / divisions;
    float deviation = length / 2;

    float textureHoriz = (1.0f / 3.0f) / divisions;
    float textureVert = (1.0f / 2.0f) / divisions;

    float downHoriz = 0, downVert = 0;
    float upHoriz = 1.0f / 3.0f, upVert = 1.0f / 2.0f - textureVert;
    float rightHoriz = (1.0f / 3.0f) * 2, rightVert = (1.0f / 2.0f) - textureVert;
    float leftHoriz = 0, leftVert = 1 - textureVert;
    float frontHoriz = 1.0f / 3.0f, frontVert = 1 - textureVert;
    float backHoriz = (1.0f / 3.0f) * 2, backVert = 1 - textureVert;

    // string que guarda os pontos que estruturam a figura
    string res = to_string(6 * (int)(pow(divisions, 2) * 6)) + "\n";

    for (int j = 0; j < divisions; j++) {
        for (int i = 0; i < divisions; i++) {
            // DOWN FACE
            p1 = to_string(i * step - deviation) + "," + to_string(-deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnd[0]) + "," + to_string(pnd[1]) + "," + to_string(pnd[2]) +  "," +
                    to_string(downHoriz) + "," + to_string(downVert) + "\n";
            p2 = to_string(i * step - deviation) + "," + to_string(-deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnd[0]) + "," + to_string(pnd[1]) + "," + to_string(pnd[2]) +  "," +
                    to_string(downHoriz + textureHoriz) + "," + to_string(downVert) + "\n";
            p3 = to_string((i + 1) * step - deviation) + "," + to_string(-deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnd[0]) + "," + to_string(pnd[1]) + "," + to_string(pnd[2]) +  "," +
                    to_string(downHoriz + textureHoriz) + "," + to_string(downVert + textureVert) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(-deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnd[0]) + "," + to_string(pnd[1]) + "," + to_string(pnd[2]) +  "," +
                    to_string(downHoriz) + "," + to_string(downVert + textureVert) + "\n";
            res = res + p1 + p3 + p2 + p3 + p1 + p4;

            // RIGHT FACE
            p1 = to_string(length - deviation) + "," + to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnr[0]) + "," + to_string(pnr[1]) + "," + to_string(pnr[2]) +  "," +
                    to_string(rightHoriz) + "," + to_string(rightVert + textureVert) + "\n";
            p2 = to_string(length - deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnr[0]) + "," + to_string(pnr[1]) + "," + to_string(pnr[2]) +  "," +
                    to_string(rightHoriz) + "," + to_string(rightVert) + "\n";
            p3 = to_string(length - deviation) + "," + to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnr[0]) + "," + to_string(pnr[1]) + "," + to_string(pnr[2]) +  "," +
                    to_string(rightHoriz + textureHoriz) + "," + to_string(rightVert) + "\n";
            p4 = to_string(length - deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnr[0]) + "," + to_string(pnr[1]) + "," + to_string(pnr[2]) +  "," +
                    to_string(rightHoriz + textureHoriz) + "," + to_string(rightVert + textureVert) + "\n";
            res = res + p1 + p2 + p3 + p4 + p2 + p1;

            // UP FACE
            p1 = to_string(i * step - deviation) + "," + to_string(length - deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnu[0]) + "," + to_string(pnu[1]) + "," + to_string(pnu[2]) +  "," +
                    to_string(upHoriz + textureHoriz) + "," + to_string(upVert) + "\n";
            p2 = to_string(i * step - deviation) + "," + to_string(length - deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnu[0]) + "," + to_string(pnu[1]) + "," + to_string(pnu[2]) +  "," +
                    to_string(upHoriz + textureHoriz) + "," + to_string(upVert + textureVert) + "\n";
            p3 = to_string((i + 1) * step - deviation) + "," + to_string(length - deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnu[0]) + "," + to_string(pnu[1]) + "," + to_string(pnu[2]) +  "," +
                    to_string(upHoriz) + "," + to_string(upVert + textureVert) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(length - deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnu[0]) + "," + to_string(pnu[1]) + "," + to_string(pnu[2]) +  "," +
                    to_string(upHoriz) + "," + to_string(upVert) + "\n";
            res = res + p1 + p2 + p3 + p4 + p1 + p3;

            // LEFT FACE
            p1 = to_string(-deviation) + "," + to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnl[0]) + "," + to_string(pnl[1]) + "," + to_string(pnl[2]) +  "," +
                    to_string(leftHoriz) + "," + to_string(leftVert + textureVert) + "\n";
            p2 = to_string(-deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," +
                    to_string(pnl[0]) + "," + to_string(pnl[1]) + "," + to_string(pnl[2]) +  "," +
                    to_string(leftHoriz) + "," + to_string(leftVert) +"\n";
            p3 = to_string(-deviation) + "," + to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnl[0]) + "," + to_string(pnl[1]) + "," + to_string(pnl[2]) +  "," +
                    to_string(leftHoriz + textureHoriz) + "," + to_string(leftVert) + "\n";
            p4 = to_string(-deviation) + "," + to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "," +
                    to_string(pnl[0]) + "," + to_string(pnl[1]) + "," + to_string(pnl[2]) +  "," +
                    to_string(leftHoriz + textureHoriz) + "," + to_string(leftVert + textureVert) + "\n";
            res = res + p1 + p2 + p3 + p2 + p4 + p3;

            // FRONT FACE
            p1 = to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(length - deviation) + "," +
                    to_string(pnf[0]) + "," + to_string(pnf[1]) + "," + to_string(pnf[2]) +  "," +
                    to_string(frontHoriz) + "," + to_string(frontVert + textureVert) + "\n";
            p2 = to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(length - deviation) + "," +
                    to_string(pnf[0]) + "," + to_string(pnf[1]) + "," + to_string(pnf[2]) +  "," +
                    to_string(frontHoriz) + "," + to_string(frontVert) + "\n";
            p3 = to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(length - deviation) + "," +
                    to_string(pnf[0]) + "," + to_string(pnf[1]) + "," + to_string(pnf[2]) +  "," +
                    to_string(frontHoriz + textureHoriz) + "," + to_string(frontVert) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(length - deviation) + "," +
                    to_string(pnf[0]) + "," + to_string(pnf[1]) + "," + to_string(pnf[2]) +  "," +
                    to_string(frontHoriz + textureHoriz) + "," + to_string(frontVert + textureVert) + "\n";
            res = res + p1 + p2 + p3 + p4 + p1 + p3;

            // BACK FACE
            p1 = to_string(i * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(-deviation) + "," +
                    to_string(pnb[0]) + "," + to_string(pnb[1]) + "," + to_string(pnb[2]) +  "," +
                    to_string(backHoriz) + "," + to_string(backVert + textureVert) + "\n";
            p2 = to_string((i + 1) * step - deviation) + "," + to_string((j + 1) * step - deviation) + "," + to_string(-deviation) + "," +
                    to_string(pnb[0]) + "," + to_string(pnb[1]) + "," + to_string(pnb[2]) +  "," +
                    to_string(backHoriz) + "," + to_string(backVert) + "\n";
            p3 = to_string(i * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(-deviation) + "," +
                    to_string(pnb[0]) + "," + to_string(pnb[1]) + "," + to_string(pnb[2]) +  "," +
                    to_string(backHoriz + textureHoriz) + "," + to_string(backVert) + "\n";
            p4 = to_string((i + 1) * step - deviation) + "," + to_string(j * step - deviation) + "," + to_string(-deviation) + "," +
                    to_string(pnb[0]) + "," + to_string(pnb[1]) + "," + to_string(pnb[2]) +  "," +
                    to_string(backHoriz + textureHoriz) + "," + to_string(backVert + textureVert) + "\n";
            res = res + p1 + p2 + p3 + p3 + p2 + p4;
        }
        downHoriz += textureHoriz; downVert = 0;
        upHoriz += textureHoriz; upVert = 1.0f / 2.0f - textureVert;
        rightHoriz = (1.0f / 3.0f) * 2; rightVert -= textureVert;
        leftHoriz = 0; leftVert -= textureVert;
        frontHoriz = 1.0f / 3.0f; frontVert -= textureVert;
        backHoriz = (1.0f / 3.0f) * 2; backVert -= textureVert;
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
    float var = (2 * radius * M_PI) / slices;

    // String onde é guardados o némero total de vertices necessários para construir o cone
    string res = to_string((2 * slices * stack) * 3) + "\n";

    if (radius < 0 || height < 0 || slices < 0 || stack < 0) {
        return false;
    }

    string file = params[4];
    int found = file.find(".3d");
    if (found <= 0) return false;

    //calcular a altura de cada stack
    double HeightofStack = height / stack;
    double normalized_y = cos(atan(height / radius));

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

            float n1x = cos(alpha);
            float n1y = normalized_y;
            float n1z = sin(alpha);

            float n2x = cos(alpha2);
            float n2y = normalized_y;
            float n2z = sin(alpha2);

            float n3x = cos(alpha);
            float n3y = normalized_y;
            float n3z = sin(alpha);

            float n4x = cos(alpha2);
            float n4y = normalized_y;
            float n4z = sin(alpha2);

            string p1 = to_string(p1x) + "," + to_string(p1y) + "," + to_string(p1z) + "," +
                    to_string(n1x) + "," + to_string(n1y) + "," + to_string(n1z) +  "," +
                    to_string(0.4375 + (0.1875 / stack) * (stack - i) * cos(alpha)) + "," +
                    to_string(0.1875 + (0.1875 / stack) * (stack - i) * sin(alpha)) + "\n";

            string p2 = to_string(p2x) + "," + to_string(p2y) + "," + to_string(p2z) + "," +
                    to_string(n2x) + "," + to_string(n2y) + "," + to_string(n2z) +  "," +
                    to_string(0.4375 + (0.1875 / stack) * (stack - i) * cos(alpha2)) + "," +
                    to_string(0.1875 + (0.1875 / stack) * (stack - i) * sin(alpha2)) + "\n";

            string p3 = to_string(p3x) + "," + to_string(p3y) + "," + to_string(p3z) + "," +
                    to_string(n3x) + "," + to_string(n3y) + "," + to_string(n3z) +  "," +
                    to_string(0.4375 + (0.1875 / stack) * (stack - (i + 1)) * cos(alpha)) + "," +
                    to_string(0.1875 + (0.1875 / stack) * (stack - (i + 1)) * sin(alpha)) + "\n";

            string p4 = to_string(p4x) + "," + to_string(p4y) + "," + to_string(p4z) + "," +
                    to_string(n4x) + "," + to_string(n4y) + "," + to_string(n4z) +  "," +
                    to_string(0.4375 + (0.1875 / stack) * (stack - (i + 1)) * cos(alpha2)) + "," +
                    to_string(0.1875 + (0.1875 / stack) * (stack - (i + 1)) * sin(alpha2)) + "\n";

            if (i == 0) {
                //Base
                string base = "0.000000,0.000000,0.000000,";
                string normalBase = to_string(0) + "," + to_string(-1) + "," + to_string(0) + "\n";
                string baseTexture = to_string(0.8125) + "," + to_string(0.1875) + "\n";

                string p1BaseTexture = to_string(0.8125 + 0.1875 * cos(alpha)) + "," + to_string(0.1875 + 0.1875 * sin(alpha)) + "\n";

                string p2BaseTexture = to_string(0.8125 + 0.1875 * cos(alpha2)) + "," + to_string(0.1875 + 0.1875 * sin(alpha2)) + "\n";

                res = res + p3 + normalBase + p1 + normalBase + p2 + normalBase + p3 + normalBase + p2 + normalBase + p4 + normalBase;
                res = res + base + normalBase + p2 + normalBase + p2BaseTexture + p1 + normalBase + p1BaseTexture;
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
/*
// Função para gerar os pontos da esfera
bool generateSphere(vector<string> params) {
    double radius = stod(params[0]);
    int totalSlices = stoi(params[1]);
    int totalStacks = stoi(params[2]);
    float textureHoriz = 1.0f / (float) totalSlices;
    float textureVert = 1.0f / (float) totalStacks;

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

    float xn0, xn1, xn2, xn3, yn0, yn1, yn2, yn3, zn0, zn1, zn2, zn3;

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
            xn0 = cos(u) * sin(v);
            yn0 = cos(v);
            zn0 = sin(u) * sin(v);
            float p0n[3] = { xn0, yn0, xn0 };
            normalize(p0n);

            x1 = cos(u) * sin(vn) * radius;
            y1 = cos(vn) * radius;
            z1 = sin(u) * sin(vn) * radius;
            xn1 = cos(u) * sin(vn);
            yn1 = cos(vn);
            zn1 = sin(u) * sin(vn);
            float p1n[3] = { xn1, yn1, xn1 };
            normalize(p1n);

            x2 = cos(un) * sin(v) * radius;
            y2 = cos(v) * radius;
            z2 = sin(un) * sin(v) * radius;
            xn2 = cos(un) * sin(v);
            yn2 = cos(v);
            zn2 = sin(un) * sin(v);
            float p2n[3] = { xn2, yn2, xn2 };
            normalize(p2n);

            x3 = cos(un) * sin(vn) * radius;
            y3 = cos(vn) * radius;
            z3 = sin(un) * sin(vn) * radius;
            xn3 = cos(un) * sin(vn);
            yn3 = cos(vn);
            zn3 = sin(un) * sin(vn);
            float p3n[3] = { xn3, yn3, xn3 };
            normalize(p3n);

            totalPoints += 6;

            string p0 = to_string(x0) + "," + to_string(y0) + "," + to_string(z0) + "," +
                    to_string(p0n[0]) + "," + to_string(p0n[1]) + "," + to_string(p0n[2]) + "," +
                    to_string(j * textureHoriz) + "," + to_string(i * textureVert + textureVert) + "\n";
            string p1 = to_string(x1) + "," + to_string(y1) + "," + to_string(z1) + "," +
                    to_string(p1n[0]) + "," + to_string(p1n[1]) + "," + to_string(p1n[2]) + "," +
                    to_string(j * textureHoriz) + "," + to_string(i * textureVert) + "\n";
            string p2 = to_string(x2) + "," + to_string(y2) + "," + to_string(z2) + "," +
                    to_string(p2n[0]) + "," + to_string(p2n[1]) + "," + to_string(p2n[2]) + "," +
                    to_string(j * textureHoriz + textureHoriz) + "," + to_string(i * textureVert) + "\n";
            string p3 = to_string(x3) + "," + to_string(y3) + "," + to_string(z3) + "," +
                    to_string(p3n[0]) + "," + to_string(p3n[1]) + "," + to_string(p3n[2]) + "," +
                    to_string(j * textureHoriz + textureHoriz) + "," + to_string(i * textureVert + textureVert) + "\n";
            aux = aux + p3 + p1 + p2 + p2 + p1 + p0;
        }
    }
    string res = to_string(totalPoints) + "\n" + aux;
    writeInFile(res, file);
    printf("File gerado com sucesso");
    return true;
}
*/
bool generateSphere(vector<string> params) {
    float radius = stod(params[0]);
    int slices = stoi(params[1]);
    int stacks = stoi(params[2]);

    string file = params[3];
    int found = file.find(".3d");
    if (found <= 0) return false;

    if (radius < 0 || slices < 0 || stacks < 0) {
        return false;
    }

    int maxTrianglePoints = 6 * slices * (stacks - 1);
    string res = to_string(maxTrianglePoints) + "\n";

    //calcute height of each stack
    float stackHeight = (2 * radius) / stacks;
    //calcute angle of each slice
    float angle = (2 * M_PI) / slices;
    float stack = 0;
    //circunference of points, given slices, height and radius
    for (float i = radius; i > 0.000001; i -= stackHeight) {

        float stackRadius = sqrt(pow(radius, 2) - pow(i, 2));
        float stackRadius2 = sqrt(pow(radius, 2) - pow((i - stackHeight), 2));

        for (int c = 0; c < slices; c++) {

            float alpha = angle * c;
            float alpha2 = angle * (c + 1);
            float p1x = cos(alpha) * stackRadius;
            float p1y = i;
            float p1z = -sin(alpha) * stackRadius;
            float p2x = cos(alpha2) * stackRadius;
            //float p2y = i;
            float p2z = -sin(alpha2) * stackRadius;
            float p3x = cos(alpha) * stackRadius2;
            float p3y = (i - stackHeight);
            float p3z = -sin(alpha) * stackRadius2;
            float p4x = cos(alpha2) * stackRadius2;
            //float p4y = (i - stackHeight);
            float p4z = -sin(alpha2) * stackRadius2;

            // Normals
            float p1n[3] = { p1x, p1y, p1z};
            float p2n[3] = { p2x, p1y, p2z };
            float p3n[3] = { p3x, p3y, p3z };
            float p4n[3] = { p4x, p3y, p4z };
            normalize( p1n );
            normalize( p2n );
            normalize( p3n );
            normalize( p4n );


            string coordinatesYMinus = to_string((-1 * i));
            string coordinatesY3Minus = to_string((-1 * (i - stackHeight)));

            string p1 = to_string(p1x) + "," + to_string(p1y) + "," + to_string(p1z) + "," +
                        to_string(p1n[0]) + "," + to_string(p1n[1]) + "," + to_string(p1n[2]) + "," +
                        to_string(alpha / (2 * M_PI)) + "," + to_string(-1*stack /stacks ) + "\n";

            string p2 = to_string(p2x) + "," + to_string(p1y) + "," + to_string(p2z) + "," +
                        to_string(p2n[0]) + "," + to_string(p2n[1]) + "," + to_string(p2n[2]) + "," +
                        to_string(alpha2 / (2 * M_PI)) + "," + to_string( -1*(stack) /stacks ) + "\n";

            string p3 = to_string(p3x) + "," + to_string(p3y) + "," + to_string(p3z) + "," +
                        to_string(p3n[0]) + "," + to_string(p3n[1]) + "," + to_string(p3n[2]) + "," +
                        to_string(alpha / (2 * M_PI)) + "," + to_string(-1* (stack+1) / stacks) + "\n";

            string p4 = to_string(p4x) + "," + to_string(p3y) + "," + to_string(p4z) + "," +
                        to_string(p4n[0]) + "," + to_string(p4n[1]) + "," + to_string(p4n[2]) + "," +
                        to_string(alpha2 / (2 * M_PI)) + "," + to_string( -1* (stack + 1) / stacks ) + "\n";


            string p1Minus = to_string(p1x) + "," + to_string((-1 * i)) + "," + to_string(p1z) + "," +
                             to_string(p1n[0]) + "," + to_string(-p1n[1]) + "," + to_string(p1n[2]) + "," +
                             to_string(alpha / (2 * M_PI)) + "," + to_string(-1*(stacks - stack) / stacks) + "\n";

            string p2Minus = to_string(p2x) + "," + to_string((-1 * i)) + "," + to_string(p2z) + "," +
                             to_string(p2n[0]) + "," + to_string(-p2n[1]) + "," + to_string(p2n[2]) + "," +
                             to_string(alpha2 / (2 * M_PI)) + "," + to_string(-1*(stacks - stack) / stacks) + "\n";

            string p3Minus = to_string(p3x) + "," + coordinatesY3Minus + "," + to_string(p3z) + "," +
                             to_string(p3n[0]) + "," + to_string(-p3n[1]) + "," + to_string(p3n[2]) + "," +
                             to_string(alpha / (2 * M_PI)) + "," + to_string(-1*(stacks - (stack + 1))/stacks) + "\n";

            string p4Minus = to_string(p4x) + "," + coordinatesY3Minus + "," + to_string(p4z) + "," +
                             to_string(p4n[0]) + "," + to_string(-p4n[1]) + "," + to_string(p4n[2]) + "," +
                             to_string(alpha2 / (2 * M_PI)) + "," + to_string(-1*(stacks - (stack + 1)) / stacks) + "\n";

            //side
            if (i != radius) {
                res = res + p1 + p3 + p4 + p1 + p4 + p2;
                //mirroring
                if (!((stacks % 2 != 0) && (i - stackHeight) < 0))
                    res = res + p1Minus + p2Minus + p3Minus + p2Minus + p4Minus + p3Minus;
            }
                //top
            else {
                res = res + p1 + p3 + p4;
                res = res + p3Minus + p1Minus + p4Minus;
            }
        }
        stack++;
    }

    writeInFile(res, file);
    return true;
}
// Função para gerar os pontos do Toro
bool generateTorus(vector<string>params) {
    float innerR = stof(params[0]);
    float outterR = stof(params[1]);
    int slices = stoi(params[2]);
    int stacks = stoi(params[3]);

    float textureHoriz = 1.0f / slices;
    float textureVert = 1.0f / stacks;

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
    float x, y, z, xn, yn, zn;

    /*
     * f.addNormalPoint(cos(phi) * cos(theta), cos(phi) * sin(theta), sin(phi));
            f.addNormalPoint(cos(phi) * cos(theta + delta1), cos(phi) * sin(theta + delta1), sin(phi));
            f.addNormalPoint(cos(phi + delta2) * cos(theta + delta1), cos(phi + delta2) * sin(theta + delta1), sin(phi + delta2));
            f.addNormalPoint(cos(phi + delta2) * cos(theta), cos(phi + delta2) * sin(theta), sin(phi + delta2));
     */
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            x = (rad + dist * cos(stepSlice * i)) * cos(stepStack * j);
            y = dist * sin(stepSlice * i);
            z = (rad + dist * cos(stepSlice * i)) * sin(stepStack * j);
            xn = cos(stepSlice * i) * cos(stepStack * j);
            yn = sin(stepSlice * i);
            zn = cos(stepSlice * i) * sin(stepStack * j);
            string p1 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "," +
                        to_string(xn) + "," + to_string(yn) + "," + to_string(zn) + "," +
                        to_string(i * textureHoriz) + "," + to_string(j * textureVert) + "\n";

            x = (rad + dist * cos(stepSlice * (i + 1))) * cos(stepStack * j);
            y = dist * sin(stepSlice * (i + 1));
            z = (rad + dist * cos(stepSlice * (i + 1))) * sin(stepStack * j);
            xn = cos(stepSlice * (i + 1)) * cos(stepStack * j);
            yn = sin(stepSlice * (i + 1));
            zn = cos(stepSlice * (i + 1)) * sin(stepStack * j);
            string p2 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "," +
                        to_string(xn) + "," + to_string(yn) + "," + to_string(zn) + "," +
                        to_string((i+1) * textureHoriz) + "," + to_string(j * textureVert) + "\n";

            x = (rad + dist * cos(stepSlice * (i + 1))) * cos(stepStack * (j + 1));
            y = dist * sin(stepSlice * (i + 1));
            z = (rad + dist * cos(stepSlice * (i + 1))) * sin(stepStack * (j + 1));
            xn = cos(stepSlice * (i + 1)) * cos(stepStack * (j + 1));
            yn = sin(stepSlice * (i + 1));
            zn = cos(stepSlice * (i + 1)) * sin(stepStack * (j + 1));
            string p3 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "," +
                        to_string(xn) + "," + to_string(yn) + "," + to_string(zn) + "," +
                        to_string((i+1) * textureHoriz) + "," + to_string((j+1) * textureVert) + "\n";

            x = (rad + dist * cos(stepSlice * i)) * cos(stepStack * (j + 1));
            y = dist * sin(stepSlice * i);
            z = (rad + dist * cos(stepSlice * i)) * sin(stepStack * (j + 1));
            xn = cos(stepSlice * i) * cos(stepStack * (j + 1));
            yn = sin(stepSlice * i);
            zn = cos(stepSlice * i) * sin(stepStack * (j + 1));
            string p4 = to_string(x) + "," + to_string(y) + "," + to_string(z) + "," +
                        to_string(xn) + "," + to_string(yn) + "," + to_string(zn) + "," +
                        to_string(i * textureHoriz) + "," + to_string((j+1) * textureVert) + "\n";

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
    float textureHoriz = 1.0f / (float) totalSlices;
    float textureVert = 1.0f / (float) totalStacks;

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

            string p0 = to_string(x0) + "," + to_string(y0) + "," + to_string(z0) +  "," +
                    to_string(j * textureHoriz) + "," + to_string(i * textureVert + textureVert) + "\n";
            string p1 = to_string(x1) + "," + to_string(y1) + "," + to_string(z1) +  "," +
                    to_string(j * textureHoriz) + "," + to_string(i * textureVert) + "\n";
            string p2 = to_string(x2) + "," + to_string(y2) + "," + to_string(z2) +  "," +
                    to_string(j * textureHoriz + textureHoriz) + "," + to_string(i * textureVert) + "\n";
            string p3 = to_string(x3) + "," + to_string(y3) + "," + to_string(z3) +  "," +
                    to_string(j * textureHoriz + textureHoriz) + "," + to_string(i * textureVert + textureVert) + "\n";
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
    Point p, normal;
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

    for (i = 0; i < coordenadas.size() / 2; i = i + 2) {
        p = coordenadas[i];
        normal = coordenadas[i + 1];
        res += to_string(p.getX()) + ", " + to_string(p.getY()) + ", " + to_string(p.getZ()) + ", " + to_string(normal.getX()) + ", " + to_string(normal.getY()) + ", " + to_string(normal.getZ()) + "\n";
    }

    string output_file = params[2];

    writeInFile(res, output_file);

    return true;
}

//------------------ FUNCTIONS PARSE INPUT ------------------//
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
    else if (primitive.compare("bezier") == 0) option = 9;

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
        if (params.size() == 3) {
            ret = generatePatch(params);
        }
        else ret = false;
        break;
    default:
        break;
    }

    return ret;
}

//------------------ MAIN ------------------//
// Function that Iniciates the Generator
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
