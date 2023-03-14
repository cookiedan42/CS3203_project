#pragma once

#include "../Storage.h"
#include "./Util.h"

using namespace Rel;

class CallsStorage : public Storage {
public:
	CallsStorage();
	void insertRelationship(Rel::Calls calls);
	RelationshipResults getAllRelationship(HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1(Arg arg1, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg2(Arg arg2, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, HasStar star = NO_STAR);
	void generateStarData();

private:
	intToStringPairMap callsMap;
	intToStringPairMap calledMap;
	intToStringPairMap callsStarMap;
	intToStringPairMap calledStarMap;
	stringPairSet globalCalls;
	stringPairSet globalCallsStar;
};