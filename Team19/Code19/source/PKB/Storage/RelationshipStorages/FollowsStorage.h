#pragma once

#include "../Storage.h"
#include "./Util.h"

using namespace Rel;

class FollowsStorage : public Storage {
public:
    FollowsStorage();
    void insertRelationship(Rel::Follows follows);
	RelationshipResults getAllRelationship(HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1(Arg arg1, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg2(Arg arg2, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, HasStar star = NO_STAR);
	void generateStarData();

private:
	intToStringPairMap followsMap;
	intToStringPairMap followedByMap;
	intToStringPairMap followsStarMap;
	intToStringPairMap followedByStarMap;
	stringPairSet globalFollows;
	stringPairSet globalFollowsStar;
};