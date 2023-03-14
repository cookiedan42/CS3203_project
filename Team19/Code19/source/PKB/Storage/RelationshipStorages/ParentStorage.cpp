#include "./ParentStorage.h"

ParentStorage::ParentStorage() {}

void ParentStorage::insertRelationship(Rel::Parent parent) {
	auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(parent.getArg1());
	auto castedArg2 = dynamic_pointer_cast<Ent::Statement>(parent.getArg2());

	Arg s1 = std::to_string(castedArg1->getStmtNo());
	Arg s2 = std::to_string(castedArg2->getStmtNo());
	stringPair relationshipPair(s1, s2);

	Util::insertIntoTable(s1, relationshipPair, parentMap);
	Util::insertIntoTable(s2, relationshipPair, childMap);
	Util::globalInsert(relationshipPair, globalParent);
}

RelationshipResults ParentStorage::getAllRelationship(HasStar star) {
	return (star) ? globalParentStar: globalParent;
}

RelationshipResults ParentStorage::getRelationshipByArg1(Arg arg1, HasStar star) {
	return (star) ? parentStarMap[arg1] : parentMap[arg1];
}

RelationshipResults ParentStorage::getRelationshipByArg2(Arg arg2, HasStar star) {
	return (star) ? childStarMap[arg2] : childMap[arg2];
}

RelationshipResults ParentStorage::getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, HasStar star) {
	return (star) ? Util::handleGetRelationshipByArg1AndArg2(parentStarMap, arg1, arg2)
				  : Util::handleGetRelationshipByArg1AndArg2(parentMap, arg1, arg2);
}

void ParentStorage::generateStarData() {
	Util::handleStar(parentMap, parentStarMap, childStarMap, globalParentStar);
}