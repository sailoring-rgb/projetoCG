#define  _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "tinyxml2.h"
#include "Primitives.h"
#include "Point.h"
#include <cstring>
#include <string>
#include <algorithm>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <io.h>
#else
#endif

using namespace tinyxml2;
using namespace std;

//Vetor para guardar nome de ficheiros
vector<string> files;

//Vetor para guardar todas as primitivas
vector<Primitive> primitives;

//Variables for keyboard fuction
float r = 10.0f;
float alpha = 0.5;
float beta = 0.5;

/**
 * Function that redimensionates a window.
 * @param w width of the window.
 * @param h height of the window.
 */
void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

bool readFile(string file) {

    ifstream Read3DFile(file);

    if (!Read3DFile) {
        return false;
    }

    string text3d;

    getline(Read3DFile, text3d);
    int vertices = stoi(text3d);

    Primitive primitive;

    for (int i = 0; i < vertices; i++) {
        vector<float> tokens;

        getline(Read3DFile, text3d);
        stringstream check1(text3d);

        string intermediate;

        while (getline(check1, intermediate, ',')) {
            tokens.push_back(stof(intermediate));
        }

        Point point;
        point.setX(tokens[0]);
        point.setY(tokens[1]);
        point.setZ(tokens[2]);

        primitive.addPoint(point);
    }

    primitives.push_back(primitive);

    Read3DFile.close();

    return true;
}

void draw() {
    glBegin(GL_TRIANGLES);

    for(int i = 0; i < primitives.size();i++)
        for (int j = 0; j < primitives[i].getNrVertices(); j++) {
            glColor3f(rand(), rand(), rand());

            Point point = primitives[i].getPoint(j);

            glVertex3f(point.getX(), point.getY(), point.getZ());
        }
    glEnd();
}

/**
 * Function that creates a scene.
 */
void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();

    gluLookAt(sin(beta) * cos(alpha) * r, sin(alpha) * r, cos(beta) * cos(alpha) * r, //camera position
        0.0, 0.0, 0.0,  // where to look
        0.0f, 1.0f, 0.0f); //up vector

    //AXIS
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Y Axis in green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis in blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

    //DRAW Instruction Here
    
    draw();

    // End of frame
    glutSwapBuffers();
}

void camera(int key_code, int x, int y) {
    float change = M_PI / 40;
    switch (key_code) {
        case GLUT_KEY_LEFT: {
            beta -= change;
            break;
        }
        case GLUT_KEY_RIGHT: {
            beta += change;
            break;
        }
        case GLUT_KEY_DOWN: {
            if (alpha > -(M_PI / 2))alpha -= change;
            break;
        }
        case GLUT_KEY_UP: {
            if (alpha > (M_PI / 2))alpha += change;
            break;
        }
    }
    glutPostRedisplay();
}



bool initGlut(int argc, char** argv){

    bool res;

    char tmp[256];

    getcwd(tmp, 256); //tmp que contem a diretoria

    string path(tmp);

    int found = path.find("ENGINE");

    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    path = path + "MODELS/";

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("ProjetoCG");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // put here the registration of the keyboard callbacks
    glutSpecialFunc(camera);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();
    return true;
}

/* Function para ler XML */
bool parseDocument() {
    bool res;

    char tmp[256];

    getcwd(tmp, 256); //tmp que contem a diretoria

    string path(tmp);

    int found = path.find("ENGINE");

    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    path = path + "MODELS/model.xml";

    strcpy(tmp, path.c_str());

    XMLDocument doc;
    doc.LoadFile(tmp);

    XMLNode* scene = doc.FirstChild();
    if (scene == nullptr) {
        cout << "ERRO";
        return false;
    }

    XMLElement* file = scene->FirstChildElement("model");

    while (file != nullptr) {
        const char* strfile;

        strfile = file->Attribute("file");
        string namefile = strfile;
        file = file->NextSiblingElement();
    }
    return true;
}

/*Main*/

int main(int argc, char** argv) {
    if (parseDocument()) {
        if (!initGlut(argc, argv)) {
            cout << "3d File Invalid";
        }
    }
    else {
        cout << "XML File Invalid";
    }
    return 1;
}