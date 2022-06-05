#ifndef Primitive_h
#define Primitive_h

#include <vector>
#include <string>
#include "Point.h"
#include "Primitive.h"

class Primitive{
	class PrimitiveBuilder;
	PrimitiveBuilder* primitiveBuilder;

public:

	Primitive();

	Primitive(std::vector<Point> vertices, std::vector<Point> normal, std::vector<Point> coordText, std::string textura, float difR, float difG, float difN,
		float ambR, float ambG, float ambB, float speR, float speG, float speB, float emiR,
		float emiG, float emiB, float shininess);

	Primitive(const Primitive& p);

	std::vector<Point> getVertices();

	std::vector<Point> getNormal();

	std::vector<Point> getCoordText();

	std::string getTextura();

	void setTextura(std::string textura);

	void setShininess(float shininess);

	int getNrVertices();

	Point getPoint(int index);

	void addPoint(Point p);

	void addNormal(Point p);

	void addCoordText(Point p);

	float getShininess();

	float getDifR(); 
	float getDifG(); 
	float getDifB();

	void setDifR(float difR); 
	void setDifG(float difG);	
	void setDifB(float difB); 

	float getAmbR(); 
	float getAmbG(); 
	float getAmbB(); 

	void setAmbR(float ambR); 
	void setAmbG(float ambG); 
	void setAmbB(float ambB); 

	float getSpeR(); 
	float getSpeG(); 
	float getSpeB();

	void setSpeR(float speR); 
	void setSpeG(float speG); 
	void setSpeB(float speB); 

	float getEmiR(); 
	float getEmiG();
	float getEmiB();

	void setEmiR(float emiR); 
	void setEmiG(float emiG); 
	void setEmiB(float emiB);

	~Primitive();
};

#endif