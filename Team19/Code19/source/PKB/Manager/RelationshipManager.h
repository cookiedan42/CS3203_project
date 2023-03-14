#pragma once

#include "../../CFG/Cfg.h"
#include "../../Request/GetCFGRequest.h"
#include "../../Request/GetRequests/GetRelationshipRequests/GetRelationshipRequest.h"
#include "../../Request/InsertRequests/InsertRelationshipRequests/InsertRelationshipRequest.h"
#include "../Storage/RelationshipStorage.h"

class RelationshipManager {
public:
	RelationshipManager();

	void insert(std::shared_ptr<InsertRelationshipRequest> request);
	void insertCFG(Cfg& cfg);

	RelationshipResults get(std::shared_ptr<GetRelationshipRequest> request);
	CFGResults get(std::shared_ptr<GetCFGRequest> request);

	void generateStarData();
	void generateNextData();

private:
	RelationshipStorage* relationshipStorage;
};