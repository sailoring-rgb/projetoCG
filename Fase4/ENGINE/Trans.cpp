#include <iterator>
#include <string>

#include "Trans.h"
using namespace std;

class Trans::TransBuilder {

private:

	string name;
	float x;
	float y;
	float z;
	float angle;
	float time;
	string align;

public:
	TransBuilder() = default;

	TransBuilder(string name, float x, float y, float z, float angle, float time, string align) {
		this->name = name;
		this->x = x;
		this->y = y;
		this->z = z;
		this->angle = angle;
		this->time = time;
		this->align = align;
	}

	std::string getName() {
		return name;
	}

	std::string getAlign() {
		return align;
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

	float getAngle() {
		return angle;
	}

	float getTime() {
		return time;
	}

	void setAlign(std::string align) {
		this->align = align;
	}

	~TransBuilder() = default;
};

Trans::Trans() : transBuilder{ new class TransBuilder() } {}

Trans::Trans(string name, float x, float y ,float z, float angle, float time, string align) : transBuilder{new TransBuilder(name, x,y,z,angle,time, align)}{}

std::string Trans::getName() {
	return transBuilder->getName();
}

float Trans::getX() {
	return transBuilder->getX();
}

float Trans::getY() {
	return transBuilder->getY();
}

float Trans::getZ() {
	return transBuilder->getZ();
}

float Trans::getAngle() {
	return transBuilder->getAngle();
}

float Trans::getTime() {
	return transBuilder->getTime();
}

std::string Trans::getAlign() {
	return transBuilder->getAlign();
}

void Trans::setAlign(std::string align) {
	transBuilder->setAlign(align);
}