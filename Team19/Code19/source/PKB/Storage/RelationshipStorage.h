#pragma once

#include "../../Relationship/Relationship.h"
#include "./RelationshipStorages/FollowsStorage.h"
#include "./RelationshipStorages/ParentStorage.h"
#include "./RelationshipStorages/CallsStorage.h"
#include "./RelationshipStorages/UsesStorage.h"
#include "./RelationshipStorages/ModifiesStorage.h"
#include "./RelationshipStorages/NextStorage.h"
#include "./CFGStorage.h"
#include "../../CFG/Cfg.h"
#include "../../TypeAlias/StorageAlias.h"

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>

using namespace Rel;

class RelationshipStorage {
public:
	RelationshipStorage();

	//STAR METHODS
	void generateStarData();

	void insertCFG(Cfg cfg);
	void generateNextData();

	void insertRelationship(Rel::Follows follows);
	void insertRelationship(Rel::Parent parent);
	void insertRelationship(Rel::Uses uses);
	void insertRelationship(Rel::Modifies modifies);
	void insertRelationship(Rel::Calls calls);

	FollowsStorage getFollowsStorage();
	ParentStorage getParentStorage();
	UsesStorage getUsesStorage();
	CallsStorage getCallsStorage();
	ModifiesStorage getModifiesStorage();
	NextStorage getNextStorage();
	CFGStorage getCFGStorage();

private:
	FollowsStorage followsStorage;
	ParentStorage parentStorage;
	UsesStorage usesStorage;
	CallsStorage callsStorage;
	ModifiesStorage modifiesStorage;
	NextStorage nextStorage;
	CFGStorage cfgStorage;
};

