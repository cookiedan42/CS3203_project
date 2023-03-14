#include "RelationshipManager.h"

RelationshipManager::RelationshipManager() {
	relationshipStorage = new RelationshipStorage;
}

void RelationshipManager::insert(std::shared_ptr<InsertRelationshipRequest> request) {
	request->execute(*relationshipStorage);
}

void RelationshipManager::insertCFG(Cfg& cfg) {
	relationshipStorage->insertCFG(cfg);
}

RelationshipResults RelationshipManager::get(std::shared_ptr<GetRelationshipRequest> request) {
	return request->execute(*relationshipStorage);
}

CFGResults RelationshipManager::get(std::shared_ptr<GetCFGRequest> request) {
	return request->execute(*relationshipStorage);
}

void RelationshipManager::generateStarData() {
	relationshipStorage->generateStarData();
}
void RelationshipManager::generateNextData() {
	relationshipStorage->generateNextData();
}
