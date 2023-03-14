#pragma once
#include <memory>
#include <vector>
#include "../../Request/InsertRequests/InsertEntityRequests/InsertEntityRequest.h"
#include "../../Request/InsertRequests/InsertRelationshipRequests/InsertRelationshipRequest.h"
#include "../../Request/InsertRequests/InsertPatternRequests/InsertPatternRequest.h"
#include "../AST/AST.h"
#include "EntityExtractor/EntityExtractor.h"
#include "PatternExtractor/PatternExtractor.h"
#include "RelationshipExtractor/RelationshipExtractor.h"
#include "CfgExtractor.h"

using namespace SP;

class DesignExtractor {
private:
	unique_ptr<ProgramNode> AST;

	vector<shared_ptr<InsertEntityRequest>> entityRequests{};
	vector<shared_ptr<InsertRelationshipRequest>> relationshipRequests{};
	vector<shared_ptr<InsertPatternRequest>> patternRequests{};

	vector<Cfg> cfgs{};

public:
	DesignExtractor(unique_ptr<ProgramNode> AST);

	vector<shared_ptr<InsertEntityRequest>> getEntityRequests() const;
	vector<shared_ptr<InsertRelationshipRequest>> getRelationshipRequests() const;
	vector<shared_ptr<InsertPatternRequest>> getPatternRequests() const;

	vector<Cfg> getCFG();

	void extractAST();
};