#pragma once

#include "./PKB.h"

#include <stdio.h>
#include "../../source/Entity/Entity.h"
#include "../TypeAlias/TypeAlias.h"

#include <unordered_set>
#include <utility>

using namespace Ent;
using namespace Rel;

class GetFacade {
public:
	GetFacade(PKB& pkb);

	RelationshipResults get(std::shared_ptr<GetRelationshipRequest> request);
	PatternResults get(std::shared_ptr<GetPatternRequest> request);
	EntityResults get(std::shared_ptr<GetEntityRequest> request);
	CFGResults get(std::shared_ptr<GetCFGRequest> request);

private:
	PKB* pkb;
};