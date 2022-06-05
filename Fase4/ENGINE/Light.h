#ifndef Light_h
#define Light_h

#include <string>
#include "Light.h"
#include "Point.h"


class Light {

	class LightBuilder;
	LightBuilder* lightBuilder;

public:

    Light();

    Light(GLenum id, std::string type, Point pos, Point dir, float cutoff);

    void setType(std::string type);

    void setCutoff(float cutoff);

    std::string getType();

    float getCutoff();

    void setPos(Point p);

    void setDir(Point d);

    Point getPos();

    Point getDir();

    void setId(GLenum id);
};

#endif