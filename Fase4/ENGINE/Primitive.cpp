#include <iterator>
#include <string>

#include <stdlib.h>
#include <iostream>
#include "Primitive.h"


using namespace std;

class Primitive::PrimitiveBuilder {

	//vari�vel privada que guarda os v�rtices
private:
	std::vector<Point> vertices;
	std::vector<Point> normal;
	std::vector<Point> coordText;
	std::string textura;
	float difR, difG, difB;
	float ambR, ambG, ambB;
	float speR, speG, speB;
	float emiR, emiG, emiB;
	float shininess;

public:

	PrimitiveBuilder() = default;

	PrimitiveBuilder(std::vector<Point> vertices, std::vector<Point> normal, std::vector<Point> coordText, std::string textura, float difR, float difG, float difN,
		float ambR, float ambG, float ambB, float speR, float speG, float speB, float emiR,
		float emiG, float emiB, float shininess) {
		for (size_t i = 0; i < vertices.size(); i++) {
			this->vertices.push_back(vertices.at(i));
		}
		for (size_t i = 0; i < normal.size(); i++) {
			this->vertices.push_back(normal.at(i));
		}
		for (size_t i = 0; i < coordText.size(); i++) {
			this->vertices.push_back(coordText.at(i));
		}
		this->textura = textura;
		this->difR = difR; this->difG = difG; this->difB = difB;
		this->ambR = ambR, this->ambG = ambG, this->ambB = ambB;
		this->speR = speR, this->speG = speG, this->speB = speB;
		this->emiR = emiR, this->emiG = emiG, this->emiB = emiB;
		this->shininess = shininess;
	}

	std::vector<Point> getVertices() {
		return vertices;
	}

	std::vector<Point> getNormal() {
		return normal;
	}

	std::vector<Point> getCoordText() {
		return coordText;
	}

	std::string getTextura() {
		return textura;
	}

	void setTextura(std::string textura) {
		this->textura = textura;
	}

	float getShininess() { return shininess; }

	void setShininess(float shininess) { this->shininess = shininess;  }

	int getNrVertices() {
		return vertices.size();
	}

	Point getPoint(int index) {
		return vertices[index];
	}

	void addPoint(Point p) {
		vertices.push_back(p);
	}

	void addNormal(Point p) {
		normal.push_back(p);
	}

	void addCoordText(Point p) {
		coordText.push_back(p);
	}

	float getDifR() { return difR; }
	float getDifG() { return difG; }
	float getDifB() { return difB; }

	void setDifR(float difR) { this->difR = difR; }
	void setDifG(float difG) { this->difG = difG; }
	void setDifB(float difB) { this->difB = difB; }

	float getAmbR() { return ambR; }
	float getAmbG() { return ambG; }
	float getAmbB() { return ambB; }

	void setAmbR(float ambR) { this->ambR = ambR; }
	void setAmbG(float ambG) { this->ambG = ambG; }
	void setAmbB(float ambB) { this->ambB = ambB; }

	float getSpeR() { return speR; }
	float getSpeG() { return speG; }
	float getSpeB() { return speB; }

	void setSpeR(float speR) { this->speR = speR; }
	void setSpeG(float speG) { this->speG = speG; }
	void setSpeB(float speB) { this->speB = speB; }

	float getEmiR() { return emiR; }
	float getEmiG() { return emiG; }
	float getEmiB() { return emiB; }

	void setEmiR(float emiR) { this->emiR = emiR; }
	void setEmiG(float emiG) { this->emiG = emiG; }
	void setEmiB(float emiB) { this->emiB = emiB; }

	~PrimitiveBuilder() = default;
};

Primitive::Primitive() : primitiveBuilder{ new class PrimitiveBuilder() } {}

Primitive::Primitive(std::vector<Point> vertices, std::vector<Point> normal, std::vector<Point> coordText,std::string textura, float difR, float difG, float difN,
					float ambR, float ambG, float ambB, float speR, float speG, float speB, float emiR,
					float emiG, float emiB, float shininess) : primitiveBuilder{ new PrimitiveBuilder(vertices,normal,coordText,textura,difR,difG,difN,
																									  ambR,ambG,ambB,speR,speG,speB,emiR,
																									  emiG,emiB,shininess) } {}

Primitive::Primitive(const Primitive& p) : primitiveBuilder{ new PrimitiveBuilder(p.primitiveBuilder->getVertices(),p.primitiveBuilder->getNormal(), 
																				  p.primitiveBuilder->getCoordText(),p.primitiveBuilder->getTextura(),
																				  p.primitiveBuilder->getDifR(),p.primitiveBuilder->getDifG(),p.primitiveBuilder->getDifB(),
																				  p.primitiveBuilder->getAmbR(),p.primitiveBuilder->getAmbG(),p.primitiveBuilder->getAmbB(),
																				  p.primitiveBuilder->getSpeR(),p.primitiveBuilder->getSpeG(),p.primitiveBuilder->getSpeB(),
																				  p.primitiveBuilder->getEmiR(),p.primitiveBuilder->getEmiG(),p.primitiveBuilder->getEmiB(),
																				  p.primitiveBuilder->getShininess())} {}

std::vector<Point> Primitive::getVertices() {
	return primitiveBuilder->getVertices();
}

std::vector<Point> Primitive::getNormal() {
	return primitiveBuilder->getNormal();
}

std::vector<Point> Primitive::getCoordText() {
	return primitiveBuilder->getCoordText();
}

void Primitive::addPoint(Point p) {
	primitiveBuilder->addPoint(p);
}

void Primitive::addNormal(Point p) {
	primitiveBuilder->addNormal(p);
}

void Primitive::addCoordText(Point p) {
	primitiveBuilder->addCoordText(p);
}

int Primitive::getNrVertices() {
	return primitiveBuilder->getNrVertices();
}

Point Primitive::getPoint(int index) {
	return primitiveBuilder->getPoint(index);
}

Primitive::~Primitive() {
	if (primitiveBuilder != nullptr) {
		delete primitiveBuilder;
		primitiveBuilder = nullptr;
	}
}

std::string Primitive::getTextura() {
	return primitiveBuilder->getTextura();
}

void Primitive::setTextura(std::string textura) {
	primitiveBuilder->setTextura(textura);
}

float Primitive::getShininess() {
	return primitiveBuilder->getShininess();
}

void Primitive::setShininess(float shininess) {
	primitiveBuilder->setShininess(shininess);
}

float Primitive::getDifR() {
	return primitiveBuilder->getDifR();
}
float Primitive::getDifG() {
	return primitiveBuilder->getDifG();
}
float Primitive::getDifB() {
	return primitiveBuilder->getDifB();
}
void Primitive::setDifR(float difR) {
	primitiveBuilder->setDifR(difR);
}
void Primitive::setDifG(float difG) {
	primitiveBuilder->setDifG(difG);
}
void Primitive::setDifB(float difB) {
	primitiveBuilder->setDifB(difB);
}


float Primitive::getAmbR() {
	return primitiveBuilder->getAmbR();
}
float Primitive::getAmbG() {
	return primitiveBuilder->getAmbG();
}
float Primitive::getAmbB() {
	return primitiveBuilder->getAmbB();
}
void Primitive::setAmbR(float ambR) {
	primitiveBuilder->setAmbR(ambR);
}
void Primitive::setAmbG(float ambG) {
	primitiveBuilder->setAmbG(ambG);
}
void Primitive::setAmbB(float ambB) {
	primitiveBuilder->setAmbB(ambB);
}


float Primitive::getSpeR() {
	return primitiveBuilder->getSpeR();
}
float Primitive::getSpeG() {
	return primitiveBuilder->getSpeG();
}
float Primitive::getSpeB() {
	return primitiveBuilder->getSpeB();
}
void Primitive::setSpeR(float speR) {
	primitiveBuilder->setSpeR(speR);
}
void Primitive::setSpeG(float speG) {
	primitiveBuilder->setSpeG(speG);
}
void Primitive::setSpeB(float speB) {
	primitiveBuilder->setSpeB(speB);
}


float Primitive::getEmiR() {
	return primitiveBuilder->getEmiR();
}
float Primitive::getEmiG() {
	return primitiveBuilder->getEmiG();
}
float Primitive::getEmiB() {
	return primitiveBuilder->getEmiB();
}
void Primitive::setEmiR(float emiR) {
	primitiveBuilder->setEmiR(emiR);
}
void Primitive::setEmiG(float emiG) {
	primitiveBuilder->setEmiG(emiG);
}
void Primitive::setEmiB(float emiB) {
	primitiveBuilder->setEmiB(emiB);
}