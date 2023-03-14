#include "DesignExtractor.h"

using namespace SP;

DesignExtractor::DesignExtractor(unique_ptr<ProgramNode> AST) : AST(move(AST)) {}

vector<shared_ptr<InsertEntityRequest>> DesignExtractor::getEntityRequests() const {
	return entityRequests;
}

vector<shared_ptr<InsertRelationshipRequest>> DesignExtractor::getRelationshipRequests() const {
	return relationshipRequests;
}

vector<shared_ptr<InsertPatternRequest>> DesignExtractor::getPatternRequests() const {
	return patternRequests;
}

vector<Cfg> DesignExtractor::getCFG() {
	return cfgs;
}

void DesignExtractor::extractAST() {
	entityRequests = EntityExtractor().extractEntities(AST);

	relationshipRequests = RelationshipExtractor().extractRelationships(AST);

	patternRequests = PatternExtractor().extractPattern(AST);

	cfgs = CfgExtractor().extractCfg(AST);
}