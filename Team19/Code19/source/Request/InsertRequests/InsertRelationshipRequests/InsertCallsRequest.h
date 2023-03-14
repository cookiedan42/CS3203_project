#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "InsertRelationshipRequest.h"
#include <vector>

class InsertCallsRequest : public InsertRelationshipRequest {
public:
	InsertCallsRequest(Rel::Calls);

	void execute(RelationshipStorage& storage) override;

private:
	Rel::Calls callsRelationship;
};