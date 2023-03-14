#pragma once

#include "./Manager/EntityManager.h"
#include "./Manager/RelationshipManager.h"
#include "./Manager/PatternManager.h"

using namespace Ent;
using namespace Rel;

class PKB {
public:
	PKB();

	RelationshipManager* getRelationshipManager();
	EntityManager* getEntityManager();
	PatternManager* getPatternManager();

private:
	EntityManager* entityManager;
	RelationshipManager* relationshipManager;
	PatternManager* patternManager;
};