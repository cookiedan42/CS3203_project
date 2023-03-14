#pragma once

#include "../Storage.h"
#include "./Util.h"

using namespace Rel;

class ParentStorage : public Storage {
public:
	ParentStorage();
	void insertRelationship(Rel::Parent parent);
	RelationshipResults getAllRelationship(HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1(Arg arg1, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg2(Arg arg2, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, HasStar star = NO_STAR);
	void generateStarData();

private:

	intToStringPairMap parentMap;
	intToStringPairMap childMap;
	intToStringPairMap parentStarMap;
	intToStringPairMap childStarMap;
	stringPairSet globalParent;
	stringPairSet globalParentStar;

};