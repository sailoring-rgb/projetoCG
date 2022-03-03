#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <io.h>
#else
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tinyxml2.h"
#include "primitives.h"
#include <vector>
#include <string>


using namespace tinyxml2;
using namespace generator;
using namespace std;

void writeInFile(string res, string file) {
    //generats XML file using tinyxml2
    char tmp[256];

    getcwd(tmp, 256); //tmp which contains the directory

    string path(tmp);

    int found = path.find("GENERATOR"); // finds generator's localization

    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    string pathXML = path + "MODELS/model.xml";

    strcpy(tmp, pathXML.c_str());

    string path3D = path + "MODELS/" + file;

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

/**
* Function that Iniciates the Generator
*/
int main(int argc, char* argv[]){
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