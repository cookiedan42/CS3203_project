#pragma once

#include "./PKB.h"
#include "../TypeAlias/TypeAlias.h"

#include <unordered_set>
#include <utility>

using namespace Ent;
using namespace Rel;

class InsertFacade {
public:
	InsertFacade(PKB &pkb);

	void insert(std::shared_ptr<InsertEntityRequest>);
	void insert(std::shared_ptr<InsertPatternRequest>);
	void insert(std::shared_ptr<InsertRelationshipRequest>);

	//Relationship methods
	void insertCFG(Cfg cfg);

	/**
	 * Generates all the necessary secondary data that can be obtained from primarily stored data.
	 * Method MUST be called only at the end of ALL relationship extraction
	 */
	void generateSecondaryData();

private:
	PKB* pkb;
};