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


public:
	TransBuilder() = default;

	TransBuilder(string name, float x, float y, float z, float angle) {
		this->name = name;
		this->x = x;
		this->y = y;
		this->z = z;
		this->angle = angle;
	}

	std::string getName() {
		return name;
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

	~TransBuilder() = default;
};

Trans::Trans() : transBuilder{ new class TransBuilder() } {}

Trans::Trans(string name, float x, float y ,float z, float angle) : transBuilder{new TransBuilder(name, x,y,z,angle)}{}

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