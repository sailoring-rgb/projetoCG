#define  _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include "tinyxml2.h"
#include "Primitive.h"
#include "Point.h"
#include <cstring>
#include <string>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include "Group.h"
#include "Trans.h"
#include "Light.h"
#include "CatmullRom.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#include <unistd.h>
#include <il.h>
#elif _WIN32
#include <direct.h>
#include <IL/il.h>
#include <io.h>
#include <GL/glew.h>
#include <GL/glut.h>
#else
#endif

using namespace tinyxml2;
using namespace std;

//Vector that stores filenames.
vector<string> files;

//Vector that stores all the groups
vector<Group> groups;

vector<Light> lights;

map<string, GLuint> texturas;

string pathFile;

//Variables needed to the keyboard function.
float x = 0.1;
float y = 0.1;
float z = 0.1;
float angle = 0.0f;
float angle2 = 0.0f;
float zoomFactor = 1.0f;
float max_zoom = 3.5f;
float min_zoom = 0.5f;
float camX = 1, camY = 1, camZ = 1;
int startX, startY, tracking = 0;
int alpha = 0, beta = 0, r = 5;
int timebase = 0, frame = 0;

GLdouble eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ;
GLdouble fov, near, far;

//int pointcount = 1;
vector<Point> catmullPoints; 
float p[5][3] = { {-1,-1,0},{-1,1,0},{1,1,0},{0,0,0},{1,-1,0} };

float prev_y[3] = { 0, 1, 0 };
float t = 0;

vector<float> vertexB, coordNormal, coordTextura;
GLuint buffers[3];
GLuint vboZone = 0;


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
    gluPerspective(fov, ratio, near, far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


string getTexturaPath(string imagem) {

    char tmp[256];

    getcwd(tmp, 256); 

    string path(tmp);

    int found = path.find("ENGINE"); 

    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    path = path + "Textures/" + imagem;

    return path;
}

/**
 * Function that reads a file, given the filename.
 * @param file filename.
 * @return bool true if everything goes well. Otherwise, returns false.
 */
Primitive readFile(string file) {

    ifstream MyReadFile(file);

    string myText;

    getline(MyReadFile, myText);
    int vertices = stoi(myText);

    Primitive primitive;

    //nós temos 3 pontos que equivale a 9 floats
    for (int i = 0; i < vertices; i++) {
        // Vector of string to save tokens
        vector<float> tokens;

        // stringstream class check1
        getline(MyReadFile, myText);
        stringstream check1(myText);

        string intermediate;

        // Tokenizing w.r.t. space ' '
        while (getline(check1, intermediate, ',')) {
            tokens.push_back(stof(intermediate));
        }

        Point point;
        point.setX(tokens[0]);
        point.setY(tokens[1]);
        point.setZ(tokens[2]);

        Point normal;
        normal.setX(tokens[3]);
        normal.setY(tokens[4]);
        normal.setZ(tokens[5]);

        Point textura;
        textura.setX(tokens[6]);
        textura.setY(tokens[7]);
        textura.setZ(0.0);

        //VBO
        vertexB.push_back(tokens[0]);
        vertexB.push_back(tokens[1]);
        vertexB.push_back(tokens[2]);

        //VBO normal
        coordNormal.push_back(tokens[3]);
        coordNormal.push_back(tokens[4]);
        coordNormal.push_back(tokens[5]);

        //VBO texturas
        coordTextura.push_back(tokens[6]);
        coordTextura.push_back(tokens[7]);
        coordTextura.push_back(0);

        primitive.addPoint(point);
        // primitive.addNormal(normal);
        // primitive.addCoordText(textura);
    }

    MyReadFile.close();

    return primitive;
}

int loadTextura(std::string s) {
    unsigned int t, tw, th;
    unsigned char* texData;
    unsigned GLuint;
    ilInit();
    glEnable(GL_TEXTURE_2D);
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)s.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();
    glGenTextures(1, &GLuint);
    glBindTexture(GL_TEXTURE_2D, GLuint);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glBindTexture(GL_TEXTURE_2D, 0);
    return GLuint;

}


/**
 * Function that draws all the primitives previously stored in a vector.
 */
void drawPrimitives(Group g) {

    string scale = "scale";
    string translate = "translate";
    string rotate = "rotate";
    string color = "color";
    string isAlign = "True";

    for (int j = 0; j < g.getNrTrans(); j++) {
        Trans t = g.getTrans(j);

        if (translate.compare(t.getName()) == 0) {
            if (t.getTime() != 0) {

                catmullPoints = g.getCatmullPoints();

                glPushMatrix();
                float p[3], d[3];
                float t1 = 100.0f;
                glBegin(GL_LINE_LOOP);
                for (int i = 0; i < t1; i += 1) {
                    CatmullRom::getGlobalCatmullRomPoint(catmullPoints, i / t1, p, d);
                    //glVertex3fv(p);
                }
                glEnd();

                glPopMatrix();

                float pos[3];
                float deriv[3];

                float timeT = (glutGet(GLUT_ELAPSED_TIME) / 1000.f) / (float)(t.getTime());
                CatmullRom::getGlobalCatmullRomPoint(g.getCatmullPoints(), timeT, (float*)pos, (float*)deriv);

                glTranslatef(pos[0], pos[1], pos[2]);     

                if (isAlign.compare(t.getAlign()) == 0) {

                    float m[4][4];
                    float x[3], y[3], z[3];

                    for (int i = 0; i < 3; i++)
                        x[i] = deriv[i];
                    CatmullRom::normalize(x);
                    CatmullRom::cross(x, prev_y, z);
                    CatmullRom::normalize(z);
                    CatmullRom::cross(z, x, y);
                    CatmullRom::normalize(x);
                    for (int i = 0; i < 3; i++)
                        prev_y[i] = y[i]; 
                    CatmullRom::buildRotMatrix(x, y, z, *m);
                    glMultMatrixf(*m);
                }                
            }
            else if (t.getTime() == 0) {
                glTranslatef(t.getX(), t.getY(), t.getZ());
            }
        }
        else if (rotate.compare(t.getName()) == 0) {
            if (t.getAngle() == 0 && t.getTime() != 0) {
                float time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
                float angle = t.getAngle() + time * (t.getTime() ? 360.f / t.getTime() : 0);
                glRotatef(angle, t.getX(), t.getY(), t.getZ());
            }
            else if (t.getAngle() != 0 && t.getTime() == 0) {
                glRotatef(t.getAngle(), t.getX(), t.getY(), t.getZ());
            }
        }
        else if (color.compare(t.getName()) == 0)
        {
            glColor3f(t.getX() / 255.f, t.getY() / 255.f, t.getZ() / 255.f);
        }
        else if (scale.compare(t.getName()) == 0) {
            glScalef(t.getX(), t.getY(), t.getZ());
        }

    }

    for (int z = 0; z < g.getNrPrimitives(); z++) {
        Primitive p = g.getPrimitives(z);

        int nrVertices = p.getNrVertices();

        if (g.getNameFile().compare("asteroids.3d") == 0) {

            int arrx[180];
            int arry[180];

            srand(1);

            for (int i = 0; i < 100; i++) {
                arrx[i] = rand() % 18 + (-5);
            }
            for (int i = 0; i < 100; i++) {
                arry[i] = rand() % 18 + (-5);
            }

            for (int i = 0; i < 180; i++) {

                glPushMatrix();
                glRotated(6 * i, 0, 1, 0);
                glTranslated(80 + arrx[i], arry[i], 0);

                glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
                glVertexPointer(3, GL_FLOAT, 0, 0);
                glDrawArrays(GL_TRIANGLES, vboZone, nrVertices);

                glPopMatrix();
            }
        }
        else {
            float dif[4] = {p.getDifR(),p.getDifG(),p.getDifB(),1.0f};
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, dif);
            glMaterialf(GL_FRONT, GL_SHININESS, p.getShininess());

            float amb[4] = {p.getAmbR(),p.getAmbG(),p.getAmbB(),1.0f};
            glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
            glMaterialf(GL_FRONT, GL_SHININESS, p.getShininess());

            float spe[4] = { p.getSpeR(),p.getSpeG(),p.getSpeB(),1.0f};
            glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
            glMaterialf(GL_FRONT, GL_SHININESS, p.getShininess());

            float emi[4] = { p.getEmiR(),p.getEmiG(),p.getEmiB(),1.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, emi);
            glMaterialf(GL_FRONT, GL_SHININESS, p.getShininess());

            if (p.getTextura().compare("null") != 0) {

                auto id = texturas.find(p.getTextura());

                glBindTexture(GL_TEXTURE_2D, id->second);

                glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
                glVertexPointer(3, GL_FLOAT, 0, 0);

                glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
                glNormalPointer(GL_FLOAT, 0, 0);

                glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
                glTexCoordPointer(2, GL_FLOAT, 0, 0);

                glDrawArrays(GL_TRIANGLES, vboZone, nrVertices);

                glBindTexture(GL_TEXTURE_2D, 0);

                vboZone = vboZone + nrVertices;
            }
        }
    }

    for (int z = 0; z < g.getNrGroups(); z++) {
        glPushMatrix();
        drawPrimitives(g.getGroup(z));
        glPopMatrix();
    }


}

/**
 * Function that creates a scene.
 */
void renderScene(void) {
    string point = "point";
    string directional = "directional";
    string spotlight = "spotlight";

    float pos[4] = { 1.0, 1.0, 1.0, 0.0 };
    float fps;
    int time;
    char s[64];

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();

    gluLookAt(eyeX * camX * zoomFactor, eyeY * camY * zoomFactor, eyeZ * camZ * zoomFactor,
        centerX, centerY, centerZ,
        upX, upY, upZ);

    glTranslatef(x, 0.0, z);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(angle2, 1.0, 0.0, 0.0);

    for (int i = 0; i < lights.size(); i++) {

        if (point.compare(lights[i].getType()) == 0) {
            float pos[4] = { lights[i].getPos().getX(), lights[i].getPos().getY(), lights[i].getPos().getZ(), 0.0 };
            GLenum idP = lights[i].getId();
            glLightfv(idP, GL_POSITION, pos);
        }
        else if (directional.compare(lights[i].getType()) == 0) {
            float dir[4] = { lights[i].getDir().getX(), lights[i].getDir().getY(), lights[i].getDir().getZ(), 1.0 };
            GLenum idD = lights[i].getId();
            glLightfv(idD, GL_POSITION, dir);
        }
        else if (spotlight.compare(lights[i].getType()) == 0) {
            // pP = lights[i].getPos();
            // pD = lights[i].getDir();
            // GLenum id = lights[i].getId();
            //float posS[3] = { p.getX(), p.getY(), p.getZ() };
            // place light
            //glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, posS);
        }
    }

    /*
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
    glEnd();*/


    //DRAW POINTS
    for (int i = 0; i < groups.size(); i++) {
        glPushMatrix();
        drawPrimitives(groups[i]);
        glPopMatrix();
    }

    vboZone = 0;

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame * 1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %f6.2", fps);
        glutSetWindowTitle(s);
    }

    // End of frame
    glutSwapBuffers();
}

/**
 * Function that reacts to events.
 * @param key_code
 * @param x
 * @param y
 */
void rodar(int key_code, int x, int y) {
    float shift = M_PI / 2;
    switch (key_code) {
    case GLUT_KEY_LEFT:
        angle -= shift;
        break;
    case GLUT_KEY_RIGHT:
        angle += shift;
        break;
    case GLUT_KEY_UP:
        angle2 += shift;
        break;
    case GLUT_KEY_DOWN:
        angle2 -= shift;
        break;
    case GLUT_KEY_F1:
        zoomFactor = std::max(zoomFactor - 0.25f, min_zoom);
        break;
    case GLUT_KEY_F2:
        zoomFactor = std::min(zoomFactor + 0.25f, max_zoom);
        break;
    }
    glutPostRedisplay();
}

void polygonMode(unsigned char key_code, int x, int y) {
    switch (key_code){
        case '1':
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case '2':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case '3':
            glPointSize(3.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy)
{
    if (state == GLUT_DOWN) {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}

void processMouseMotion(int xx, int yy)
{
    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {

        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * sin(betaAux * 3.14 / 180.0);
}

/**
 * Function that inits glut.
 * @param argc size of array.
 * @param argv array with arguments.
 * @return bool true if everything goes well. Otherwise, returns false.
 */
bool initGlut(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("TP-CG");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);    

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(polygonMode);
    glutMouseFunc(processMouseButtons);
    glutSpecialFunc(rodar);
    glutMotionFunc(processMouseMotion);

    #ifndef __APPLE__
        glewInit();
    #endif  

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_LINE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glClearColor(0, 0, 0, 0);

    // enable light
    glEnable(GL_LIGHTING);
    for (int i = 0; i < lights.size(); i++) {
        glEnable(GL_LIGHT0 + i);
    }
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_TEXTURE_2D);

    //Create VBO
    glGenBuffers(3, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexB.size(), &(vertexB[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coordNormal.size(), &(coordNormal[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coordTextura.size(), &(coordTextura[0]), GL_STATIC_DRAW);

    std::map<std::string, GLuint>::iterator it = texturas.begin();

    while (it != texturas.end())
    {
        std::string file = it->first;
        it->second = loadTextura(file);
        it++;
    }


    // enter GLUT's main cycle
    glutMainLoop();
    return true;
}


Group parseGroup(XMLElement* group, int father) {
    string models = "models";
    string scale = "scale";
    string translate = "translate";
    string rotate = "rotate";
    string grupo = "group";
    string color = "color";
    string transform = "transform";
    string model = "model";
    string texture = "texture";
    string diffuse = "diffuse";
    string ambient = "ambient";
    string specular = "specular";
    string emissive = "emissive";
    string shininess = "shininess";

    Group g;

    do {
        g = Group();
        XMLElement* element = group->FirstChildElement();
                
        while (element != nullptr) {

            if (models.compare(element->Name()) == 0) {

                XMLElement* file = element->FirstChildElement();
          
                while (file != nullptr) {

                    const char* strfile = file->Attribute("file");
                    string namefile = strfile;
                    string path = pathFile + namefile;
                    Primitive primitive = readFile(path);

                    g.setNameFile(namefile);

                    XMLElement* model = file->FirstChildElement();

                    while (model != nullptr) {
                        if (texture.compare(model->Name()) == 0) {
                            string textura = model->Attribute("file");
                            
                            if (textura.size() != 0) {
                                string pathImg = getTexturaPath(textura);
                                texturas.insert(pair<string, GLuint>(pathImg, -1));
                                primitive.setTextura(pathImg);
                            }
                            else primitive.setTextura("null");
                        }
                        else if (color.compare(model->Name()) == 0) {
                            XMLElement* color = model->FirstChildElement();

                            while (color != nullptr) {
                                
                                if (diffuse.compare(color->Name()) == 0) {
                                    if (color != nullptr) {
                                        float difR = atof(color->Attribute("R"));
                                        float difG = atof(color->Attribute("G"));
                                        float difB = atof(color->Attribute("B"));

                                        primitive.setDifR(difR);
                                        primitive.setDifG(difG);
                                        primitive.setDifB(difB);
                                    }
                                }
                                else if (ambient.compare(color->Name()) == 0) {
                                    if (color != nullptr) {
                                        float ambR = atof(color->Attribute("R"));
                                        float ambG = atof(color->Attribute("G"));
                                        float ambB = atof(color->Attribute("B"));

                                        primitive.setAmbR(ambR);
                                        primitive.setAmbG(ambG);
                                        primitive.setAmbB(ambB);
                                    }
                                }
                                else if (specular.compare(color->Name()) == 0) {
                                    if (color != nullptr) {
                                        float speR = atof(color->Attribute("R"));
                                        float speG = atof(color->Attribute("G"));
                                        float speB = atof(color->Attribute("B"));

                                        primitive.setSpeR(speR);
                                        primitive.setSpeG(speG);
                                        primitive.setSpeB(speB);
                                    }                                
                                }
                                else if (emissive.compare(color->Name()) == 0) {
                                    if (color != nullptr) {
                                        float emiR = atof(color->Attribute("R"));
                                        float emiG = atof(color->Attribute("G"));
                                        float emiB = atof(color->Attribute("B"));

                                        primitive.setEmiR(emiR);
                                        primitive.setEmiG(emiG);
                                        primitive.setEmiB(emiB);
                                    }
                                }
                                else if (shininess.compare(color->Name()) == 0) {
                                    if (color != nullptr) {
                                        float value = atof(color->Attribute("value"));

                                        primitive.setShininess(value);
                                    }
                                }
                                color = color->NextSiblingElement();
                            }
                        }
                        model = model->NextSiblingElement();
                    }
                    g.addPrimitives(primitive);
                    file = file->NextSiblingElement();
                }
            }
            else if (transform.compare(element->Name()) == 0){
                
                XMLElement* transformation = element->FirstChildElement();

                while(transformation != nullptr){
                    if (scale.compare(transformation->Name()) == 0) {
                        if (transformation != nullptr) {
                            float x = atof(transformation->Attribute("x"));
                            float y = atof(transformation->Attribute("y"));
                            float z = atof(transformation->Attribute("z"));
                            Trans t = Trans("scale", x, y, z, 0, 0, "");
                            g.addTrans(t);
                        }
                    }
                    else if(translate.compare(transformation->Name()) == 0) {
                        if (transformation != nullptr) {

                            if (transformation->FindAttribute("time")) {
                                float time = atof(transformation->Attribute("time"));
                                string align = transformation->Attribute("align");

                                XMLElement* point = transformation->FirstChildElement("point");
                                while (point != nullptr) {
                                    float x = atof(point->Attribute("x"));
                                    float y = atof(point->Attribute("y"));
                                    float z = atof(point->Attribute("z"));

                                    Point p;
                                    p.setX(x);
                                    p.setY(y);
                                    p.setZ(z);

                                    g.addCatmullPoint(p);

                                    point = point->NextSiblingElement("point");
                                }

                                Trans t = Trans("translate", 0, 0, 0, 0, time,align);
                                g.addTrans(t);
                            }
                            else if (transformation->FindAttribute("x")) {
                                float x = atof(transformation->Attribute("x"));
                                float y = atof(transformation->Attribute("y"));
                                float z = atof(transformation->Attribute("z"));

                                Trans t = Trans("translate", x, y, z, 0, 0,"");
                                g.addTrans(t);
                            }
                        }
                    }
                    else if (rotate.compare(transformation->Name()) == 0) {
                            if (transformation != nullptr) {                              
                                float x = atof(transformation->Attribute("x"));
                                float y = atof(transformation->Attribute("y"));
                                float z = atof(transformation->Attribute("z"));

                                if (transformation->FindAttribute("time")) {
                                    float time = atof(transformation->Attribute("time"));
                                    Trans t = Trans("rotate", x, y, z, 0, time,"");
                                    g.addTrans(t);
                                }
                                else if (transformation->FindAttribute("angle")) {
                                    float angle = atof(transformation->Attribute("angle"));
                                    Trans t = Trans("rotate", x, y, z, angle, 0,"");
                                    g.addTrans(t);
                                }
                            }
                    }
                    else if (color.compare(transformation->Name()) == 0) {
                            if (transformation != nullptr) {
                                float red = atof(transformation->Attribute("x"));
                                float green = atof(transformation->Attribute("y"));
                                float blue = atof(transformation->Attribute("z"));
                                Trans t = Trans("color", red, green, blue, 0, 0,"");
                                g.addTrans(t);
                            }
                    }
                    transformation = transformation->NextSiblingElement();
                }
            }
            else if (grupo.compare(element->Name()) == 0) {
                Group gr = parseGroup(element, 1);
                g.addGroups(gr);
            }
            element = element->NextSiblingElement();
            if (element == NULL && father == 1) return g;
        }
        if (father == 0) groups.push_back(g);
        group = group->NextSiblingElement();

    } while (group != nullptr);

    return g;
}

void parseCamera(XMLElement* camera) {
    string position = "position";
    string lookAt = "lookAt";
    string up = "up";
    string projection = "projection";

    XMLElement* element = camera->FirstChildElement();
     
    while (element != nullptr) {

        if (position.compare(element->Name()) == 0) {
            if (element != nullptr) {
                eyeX = stod(element->Attribute("x"));
                eyeY = stod(element->Attribute("y"));
                eyeZ = stod(element->Attribute("z"));
            }
        }
        else if (lookAt.compare(element->Name()) == 0) {
            if (element != nullptr) {
                centerX = stod(element->Attribute("x"));
                centerY = stod(element->Attribute("y"));
                centerZ = stod(element->Attribute("z"));
            }
        }
        else if (up.compare(element->Name()) == 0) {
            if (element != nullptr) {
                upX = stod(element->Attribute("x"));
                upY = stod(element->Attribute("y"));
                upZ = stod(element->Attribute("z"));
            }
        }
        else if (projection.compare(element->Name()) == 0) {
            if (element != nullptr) {
                fov = stod(element->Attribute("fov"));
                near = stod(element->Attribute("near"));
                far = stod(element->Attribute("far"));
            }
        }

        element = element->NextSiblingElement();
    }        
}

void parseLights(XMLElement* light) {
    string lig = "light";
    string point = "point";
    string directional = "directional";
    string spotlight = "spotlight";
    Point p;
    p.setX(0);
    p.setY(0);
    p.setZ(0);

    XMLElement* element = light->FirstChildElement();

    int light_counter = 0;

    while (element != nullptr) {
                
        Light l = Light();

        GLenum l1 = GL_LIGHT0 + light_counter;

        if (lig.compare(element->Name()) == 0) {
            string type = element->Attribute("type");
            l.setType(type);    
            
            if (point.compare(type) == 0) {
                float posX = stod(element->Attribute("posx"));
                float posY = stod(element->Attribute("posy"));
                float posZ = stod(element->Attribute("posz"));

                Point pos;
                pos.setX(posX);
                pos.setY(posY);
                pos.setZ(posZ);
                l.setPos(pos);
                l.setDir(p);
                l.setCutoff(0.0);
                
                l.setId(l1);
                lights.push_back(l);
                light_counter++;
            }
            else if (directional.compare(type) == 0) {
                float dirX = stod(element->Attribute("dirx"));
                float dirY = stod(element->Attribute("diry"));
                float dirZ = stod(element->Attribute("dirz"));

                Point dir;
                dir.setX(dirX);
                dir.setY(dirY);
                dir.setZ(dirZ);
                l.setDir(dir);
                l.setPos(p);
                l.setCutoff(0.0);

                l.setId(l1);
                lights.push_back(l);
                light_counter++;
            }
            else if (spotlight.compare(type) == 0) {
                float posX = stod(element->Attribute("posx"));
                float posY = stod(element->Attribute("posy"));
                float posZ = stod(element->Attribute("posz"));
                float dirX = stod(element->Attribute("dirx"));
                float dirY = stod(element->Attribute("diry"));
                float dirZ = stod(element->Attribute("dirz"));
                float cutoff = stod(element->Attribute("cutoff"));
                
                Point pos,dir;
                pos.setX(posX);
                pos.setY(posY);
                pos.setZ(posZ);
                l.setPos(pos);
                dir.setX(dirX);
                dir.setY(dirY);
                dir.setZ(dirZ);
                l.setDir(dir);
                l.setCutoff(cutoff);

                l.setId(l1);
                lights.push_back(l);
                light_counter++;
            }
        }
        element = element->NextSiblingElement();
    }
}

/**
 * Function that reads a XML file.
 * @return bool true if everything goes well. Otherwise, returns false.
 */
bool parseDocument() {

    char tmp[256];

    getcwd(tmp, 256); //tmp which contains the directory

    string path(tmp);

    int found = path.find("ENGINE"); 

    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    string fileName;

    cout << "Introduza o nome do ficheiro XML: " << endl;
    cin >> fileName;
    path = path + "Models/" + fileName + ".xml";

    strcpy(tmp, path.c_str());

    XMLDocument doc;
    doc.LoadFile(tmp);

    XMLElement* world = doc.FirstChildElement();
    if (world == nullptr) {
        cout << "ERRO";
        return false; 
    }

    XMLElement* camera = world->FirstChildElement("camera");
    parseCamera(camera);

    XMLElement* lights = world->FirstChildElement("lights");
    parseLights(lights);

    XMLElement* group = world->FirstChildElement("group");
    parseGroup(group, 0);

    return true;
}

/**
 * Main Function.
 * @param argc size of array.
 * @param argv array with arguments.
 * @return int
 */
int main(int argc, char** argv) {

    bool res = true;
    char tmp[256];

    getcwd(tmp, 256);

    string path(tmp);
    vertexB.reserve(100000);

    int found = path.find("ENGINE");

    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    path = path + "Models/";

    pathFile = path;

    if (parseDocument()) {
        initGlut(argc, argv);
    }
    else {
        cout << "XML File Invalid";
    }
    return 1;
}