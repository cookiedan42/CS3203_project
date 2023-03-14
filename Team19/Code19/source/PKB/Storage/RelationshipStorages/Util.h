#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <stack>
#include <iostream>

#include "../../../Entity/Entity.h"
#include "../../../Relationship/Relationship.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../CFGStorage.h"


class Util {
public:
	Util();
	static void insertIntoTable(std::string key, stringPair relationshipPair, intToStringPairMap& storage);
	static void globalInsert(stringPair relationship, stringPairSet& globalSet);
	static RelationshipResults handleGetRelationshipByArg1AndArg2(intToStringPairMap& map, std::string arg1, std::string arg2);
	static void handleStar(intToStringPairMap& defaultMap, intToStringPairMap& starMap, intToStringPairMap& reverseMap, stringPairSet& globalSet);
	static void handleGenerateNextData(CFGStorage& cfgStorage, intToStringPairMap& nextMap, intToStringPairMap& nextByMap, stringPairSet& globalNext);

	//Unstorable star methods
	static RelationshipResults getStar(CFGStorage &cfgStorage);
	static RelationshipResults getStarByArg1(CFGStorage& cfgStorage, Arg arg1);
	static RelationshipResults getStarByArg2(CFGStorage& cfgStorage, Arg arg2);
	static RelationshipResults getStarByArg1AndArg2(CFGStorage& cfgStorage, Arg arg1, Arg arg2);
};
