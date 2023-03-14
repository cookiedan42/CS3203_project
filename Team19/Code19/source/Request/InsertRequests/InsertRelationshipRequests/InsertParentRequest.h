#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "InsertRelationshipRequest.h"
#include <vector>

class InsertParentRequest : public InsertRelationshipRequest {
public:
	InsertParentRequest(Rel::Parent);

	void execute(RelationshipStorage& storage) override;

private:
	Rel::Parent parentRel;
};