#include "RelationshipExtractor.h"

RelationshipExtractor::RelationshipExtractor() {}

vector<shared_ptr<InsertRelationshipRequest>> RelationshipExtractor::extractRelationships(const unique_ptr<ProgramNode>& AST) {
	auto callRelationships = CallsExtractor().extractCalls(AST);
	for (const auto& rel : callRelationships)
		relationshipRequests.push_back(make_shared<InsertCallsRequest>(rel));

	auto followsRelationships = FollowsExtractor().extractFollows(AST);
	for (const auto& rel : followsRelationships)
		relationshipRequests.push_back(make_shared<InsertFollowsRequest>(rel));

	auto modifiesRelationships = ModifiesExtractor().extractModifies(AST);
	for (const auto& rel : modifiesRelationships)
		relationshipRequests.push_back(make_shared<InsertModifiesRequest>(rel));

	auto parentRelationships = ParentExtractor().extractParent(AST);
	for (const auto& rel : parentRelationships)
		relationshipRequests.push_back(make_shared<InsertParentRequest>(rel));

	auto usesRelationships = UsesExtractor().extractUses(AST);
	for (const auto& rel : usesRelationships)
		relationshipRequests.push_back(make_shared<InsertUsesRequest>(rel));

	return move(relationshipRequests);
}