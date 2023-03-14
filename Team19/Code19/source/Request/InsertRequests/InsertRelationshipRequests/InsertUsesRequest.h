#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "InsertRelationshipRequest.h"
#include <vector>

class InsertUsesRequest : public InsertRelationshipRequest {
public:
	InsertUsesRequest(Rel::Uses);

	void execute(RelationshipStorage& storage) override;

private:
	Rel::Uses usesRel;
};