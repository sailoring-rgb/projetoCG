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

    Light(std::string type, Point coord);

    void setType(std::string type);

    std::string getType();

    void setCoord(Point p);

    Point getCoord();

    ~Light();

};

#endif