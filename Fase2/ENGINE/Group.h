#ifndef Group_h
#define Group_h

#include <vector>
#include "Group.h"
#include "Primitive.h"
#include "Trans.h"

class Group {
	class GroupBuilder;
	GroupBuilder* groupBuilder;

public:

	Group();

	Group(std::vector<Primitive> primitives, std::vector<Group> groups, std::vector<Trans> trans, std::string nameFile);

	int getNrPrimitives();

	int getNrGroups();

	int getNrTrans();

	void addTrans(Trans t);

	void addPrimitives(Primitive p);

	void addGroups(Group g);

	void setPrimitives(std::vector<Primitive> primitives);

	void setNameFile(std::string nameFile);

	Trans getTrans(int index);

	Primitive getPrimitives(int index);

	std::string getNameFile();

	std::vector<Group> getGroups();

	Group getGroup(int index);

	~Group();

};

#endif