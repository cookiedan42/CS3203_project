#pragma once
#include <memory>
#include <vector>
#include "../../../Request/InsertRequests/InsertRelationshipRequests/InsertCallsRequest.h"
#include "../../../Request/InsertRequests/InsertRelationshipRequests/InsertFollowsRequest.h"
#include "../../../Request/InsertRequests/InsertRelationshipRequests/InsertModifiesRequest.h"
#include "../../../Request/InsertRequests/InsertRelationshipRequests/InsertParentRequest.h"
#include "../../../Request/InsertRequests/InsertRelationshipRequests/InsertRelationshipRequest.h"
#include "../../../Request/InsertRequests/InsertRelationshipRequests/InsertUsesRequest.h"
#include "../../AST/AST.h"
#include "CallsExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "UsesExtractor.h"

using namespace SP;
using namespace std;

class RelationshipExtractor {
private:
	vector<shared_ptr<InsertRelationshipRequest>> relationshipRequests{};

public:
	RelationshipExtractor();

	vector<shared_ptr<InsertRelationshipRequest>> extractRelationships(const unique_ptr<ProgramNode>& AST);
};

