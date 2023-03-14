#include "./FollowsStorage.h"

FollowsStorage::FollowsStorage() {}

void FollowsStorage::insertRelationship(Rel::Follows follows) {
	auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(follows.getArg1());
	auto castedArg2 = dynamic_pointer_cast<Ent::Statement>(follows.getArg2());

	Arg s1 = std::to_string(castedArg1->getStmtNo());
	Arg s2 = std::to_string(castedArg2->getStmtNo());
	stringPair relationshipPair(s1, s2);

	Util::insertIntoTable(s1, relationshipPair, followsMap);
	Util::insertIntoTable(s2, relationshipPair, followedByMap);
	Util::globalInsert(relationshipPair, globalFollows);
 }

RelationshipResults FollowsStorage::getAllRelationship(HasStar star) {
	return (star) ? globalFollowsStar : globalFollows;
}

RelationshipResults FollowsStorage::getRelationshipByArg1(Arg arg1, HasStar star) {
	return (star) ? followsStarMap[arg1] : followsMap[arg1];
}

RelationshipResults FollowsStorage::getRelationshipByArg2(Arg arg2, HasStar star) {
	return (star) ? followedByStarMap[arg2] : followedByMap[arg2];
}

RelationshipResults FollowsStorage::getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, HasStar star) {
	return (star) ? Util::handleGetRelationshipByArg1AndArg2(followsStarMap, arg1, arg2)
				  : Util::handleGetRelationshipByArg1AndArg2(followsMap, arg1, arg2);
}

void FollowsStorage::generateStarData() {
	Util::handleStar(followsMap, followsStarMap, followedByStarMap, globalFollowsStar);
}