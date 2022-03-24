#ifndef GROUP_h
#define GROUP_h

#include <vector>
#include "GROUP.h"
#include "Primitive.h"
#include "Trans.h"

class Group {
	class GroupBuiler;


public:

	Group();

	Group(std::vector<Primitive> primitives, std::vector<Group> groups, std::vector<Trans> Trans);

	int getNrPrimitives();

	int getNrGroups();

	int getNrTrans();

	void addTrans(Trans t);

	void addPrimitives(Primitive p);

	void addGroups(Group g);

	void setPrimitives(std::vector<Primitive> primitives);

	Trans getTrans(int index);

	Primitive getPrimitives(int index);

	std::vector<Group> getGroups();

	Group getGroup(int index);

	~Group();
};

#endif