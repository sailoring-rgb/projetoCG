#include <cmath>
#include <iterator>
#include <string>

#include "Light.h"
#include "Point.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include <io.h>
#include <GL/glew.h>
#include <GL/glut.h>
#else
#endif


class Light::LightBuilder {

    private:
        std::string type;
        Point coord;

    public:
        LightBuilder() = default;

        LightBuilder(std::string type, Point coord) {
            this->type = type;
            this->coord = coord;
        }

        void setType(std::string type) {
            this->type = type;
        }

        std::string getType() {
            return type;
        }

        void setCoord(Point p) {
            coord.setX(p.getX());
            coord.setY(p.getY());
            coord.setZ(p.getZ());
        }

        Point getCoord() {
            return coord;
        }

        ~LightBuilder() = default;
};

Light::Light() : lightBuilder{ new class LightBuilder() } {}

Light::Light(std::string type, Point coord) : lightBuilder{ new class LightBuilder(type,coord) } {}

void Light::setType(std::string type) {
    lightBuilder->setType(type);
}

void Light::setCoord(Point coord) {
    lightBuilder->setCoord(coord);
}

std::string Light::getType() {
    return lightBuilder->getType();
}

Point Light::getCoord() {
    return lightBuilder->getCoord();
}



