#include "PKB.h"

PKB::PKB() {
	entityManager = new EntityManager;
	relationshipManager = new RelationshipManager;
	patternManager = new PatternManager;
}

RelationshipManager* PKB::getRelationshipManager() {
	return relationshipManager;
}

EntityManager* PKB::getEntityManager() {
	return entityManager;
}

PatternManager* PKB::getPatternManager() {
	return patternManager;
}