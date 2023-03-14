#include "RelationshipStorage.h"

RelationshipStorage::RelationshipStorage() {}

void RelationshipStorage::insertRelationship(Rel::Follows follows) {
	followsStorage.insertRelationship(follows);
}

void RelationshipStorage::insertRelationship(Rel::Parent parent) {
	parentStorage.insertRelationship(parent);
}

void RelationshipStorage::insertRelationship(Rel::Uses uses) {
	usesStorage.insertRelationship(uses);
}

void RelationshipStorage::insertRelationship(Rel::Modifies modifies) {
	modifiesStorage.insertRelationship(modifies);
}

void RelationshipStorage::insertRelationship(Rel::Calls calls) {
	callsStorage.insertRelationship(calls);
}

//STAR METHODS
void RelationshipStorage::generateStarData() {
	followsStorage.generateStarData();
	parentStorage.generateStarData();
	callsStorage.generateStarData();
}

void RelationshipStorage::insertCFG(Cfg cfg) {
	cfgStorage.insertCFG(cfg);
}

void RelationshipStorage::generateNextData() {
	nextStorage.generateNextData(cfgStorage);
}

FollowsStorage RelationshipStorage::getFollowsStorage() {
	return followsStorage;
}

ParentStorage RelationshipStorage::getParentStorage() {
	return parentStorage;
};

UsesStorage RelationshipStorage::getUsesStorage() {
	return usesStorage;
};

CallsStorage RelationshipStorage::getCallsStorage() {
	return callsStorage;
};

ModifiesStorage RelationshipStorage::getModifiesStorage() {
	return modifiesStorage;
};

NextStorage RelationshipStorage::getNextStorage() {
	return nextStorage;
};

CFGStorage RelationshipStorage::getCFGStorage() {
	return cfgStorage;
};