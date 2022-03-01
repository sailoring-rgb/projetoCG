#ifndef FASE1_PRIMITIVES_H
#define FASE1_PRIMITIVES_H

#include <vector>
#include "Point.h"


class Primitive {
	class PrimitiveBuilder;
	PrimitiveBuilder* primitiveBuilder;


public:

	Primitive();

	Primitive(std::vector<Point> vertices);

	Primitive(const Primitive& p);

	std::vector<Point> getVertices();

	int getNrVertices();

	Point getPoint(int index);

	void addPoint(Point p);

	~Primitive();
};
#endif //FASE1_PRIMITIVES_H