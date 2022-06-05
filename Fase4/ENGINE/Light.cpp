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
        GLenum id;
        std::string type;
        Point pos;
        Point dir;
        float cutoff;

    public:
        LightBuilder() = default;

        LightBuilder(GLenum id, std::string type, Point pos, Point dir, float cutoff) {
            this->id = id;
            this->type = type;
            this->pos = pos;
            this->dir = dir;
            this->cutoff = cutoff;
        }

        void setId(GLenum id) {
            this->id = id;
        }

        GLenum getId() {
            return this->id;
        }

        void setType(std::string type) {
            this->type = type;
        }

        std::string getType() {
            return type;
        }

        void setCutoff(float cutoff) {
            this->cutoff = cutoff;
        }

        float getCutoff() {
            return cutoff;
        }

        void setPos(Point p) {
            pos.setX(p.getX());
            pos.setY(p.getY());
            pos.setZ(p.getZ());
        }

        Point getPos() {
            return pos;
        }

        void setDir(Point d) {
            dir.setX(d.getX());
            dir.setY(d.getY());
            dir.setZ(d.getZ());
        }

        Point getDir() {
            return dir;
        }

        ~LightBuilder() = default;
};

Light::Light() : lightBuilder{ new class LightBuilder() } {}

Light::Light(GLenum id, std::string type, Point pos, Point dir, float cutoff) : lightBuilder{ new class LightBuilder(id, type, pos, dir, cutoff) } {}

void Light::setType(std::string type) {
    lightBuilder->setType(type);
}

void Light::setCutoff(float cutoff) {
    lightBuilder->setCutoff(cutoff);
}

void Light::setPos(Point pos) {
    lightBuilder->setPos(pos);
}

void Light::setDir(Point dir) {
    lightBuilder->setDir(dir);
}

std::string Light::getType() {
    return lightBuilder->getType();
}

float Light::getCutoff() {
    return lightBuilder->getCutoff();
}

Point Light::getPos() {
    return lightBuilder->getPos();
}

Point Light::getDir() {
    return lightBuilder->getDir();
}

void Light::setId(GLenum id) {
    lightBuilder->setId(id);
}

GLenum Light::getId() {
    return lightBuilder->getId();
}

