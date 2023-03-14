#pragma once

#include "../Storage.h"
#include "../CFGStorage.h"
#include "./Util.h"

using namespace Rel;

class NextStorage : public Storage {
public:
	NextStorage();
	RelationshipResults getAllRelationship(CFGStorage& cfgStorage, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1(CFGStorage& cfgStorage, Arg arg1, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg2(CFGStorage& cfgStorage, Arg arg2, HasStar star = NO_STAR);
	RelationshipResults getRelationshipByArg1AndArg2(CFGStorage& cfgStorage, Arg arg1, Arg arg2, HasStar star = NO_STAR);
	void generateNextData(CFGStorage& cfgStorage);

private:
	intToStringPairMap nextMap;
	intToStringPairMap nextByMap;
	stringPairSet globalNext;
};