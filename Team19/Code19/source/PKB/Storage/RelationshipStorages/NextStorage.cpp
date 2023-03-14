#include "./NextStorage.h"

NextStorage::NextStorage() {}

RelationshipResults NextStorage::getAllRelationship(CFGStorage& cfgStorage, HasStar star) {
	return (star) ? Util::getStar(cfgStorage)
				  : globalNext;
}

RelationshipResults NextStorage::getRelationshipByArg1(CFGStorage& cfgStorage, Arg arg1, HasStar star) {
	return (star) ? Util::getStarByArg1(cfgStorage, arg1)
				  : nextMap[arg1];
}

RelationshipResults NextStorage::getRelationshipByArg2(CFGStorage& cfgStorage, Arg arg2, HasStar star) {
	return (star) ? Util::getStarByArg2(cfgStorage, arg2)
				  : nextByMap[arg2];
}

RelationshipResults NextStorage::getRelationshipByArg1AndArg2(CFGStorage& cfgStorage, Arg arg1, Arg arg2, HasStar star) {
	return (star) ? Util::getStarByArg1AndArg2(cfgStorage, arg1, arg2)
			      : Util::handleGetRelationshipByArg1AndArg2(nextMap, arg1, arg2);
}

void NextStorage::generateNextData(CFGStorage& cfgStorage) {
	Util::handleGenerateNextData(cfgStorage, nextMap, nextByMap, globalNext);
}
