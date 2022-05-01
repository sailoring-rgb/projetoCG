#ifndef Trans_h
#define Trans_h

#include <string>
#include "Trans.h"

class Trans{
	class TransBuilder;
	TransBuilder* transBuilder;

public:

	Trans();

	Trans(std::string name, float x, float y, float z, float angle, float time, std::string align);

	std::string getName();

	float getX();

	float getY();

	float getZ();

	float getAngle();

	float getTime();

	void setAlign(std::string align);

	std::string getAlign();
};


#endif