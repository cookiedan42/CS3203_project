#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "InsertRelationshipRequest.h"
#include <vector>

class InsertFollowsRequest : public InsertRelationshipRequest {
public:
	InsertFollowsRequest(Rel::Follows);

	void execute(RelationshipStorage& storage) override;

private:
	Rel::Follows followsRelationship;
};