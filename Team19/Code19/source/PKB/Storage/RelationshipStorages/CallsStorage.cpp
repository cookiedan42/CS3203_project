#include "./CallsStorage.h"

CallsStorage::CallsStorage() {}

void CallsStorage::insertRelationship(Rel::Calls calls) {
	Arg p1 = calls.getArg1()->getName();
	Arg p2 = calls.getArg2()->getName();
	stringPair relationshipPair(p1, p2);

	Util::insertIntoTable(p1, relationshipPair, callsMap);
	Util::insertIntoTable(p2, relationshipPair, calledMap);
	Util::globalInsert(relationshipPair, globalCalls);
}

RelationshipResults CallsStorage::getAllRelationship(HasStar star) {
	return (star) ? globalCallsStar : globalCalls;
}

RelationshipResults CallsStorage::getRelationshipByArg1(Arg arg1, HasStar star) {
	return (star) ? callsStarMap[arg1] : callsMap[arg1];
}

RelationshipResults CallsStorage::getRelationshipByArg2(Arg arg2, HasStar star) {
	return (star) ? calledStarMap[arg2] : calledMap[arg2];
}

RelationshipResults CallsStorage::getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, HasStar star) {
	return (star) ? Util::handleGetRelationshipByArg1AndArg2(callsStarMap, arg1, arg2)
			      : Util::handleGetRelationshipByArg1AndArg2(callsMap, arg1, arg2);
}

void CallsStorage::generateStarData() {
	Util::handleStar(callsMap, callsStarMap, calledStarMap, globalCallsStar);
}