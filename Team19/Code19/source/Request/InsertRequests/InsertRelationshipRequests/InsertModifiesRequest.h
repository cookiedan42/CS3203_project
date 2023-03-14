#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "InsertRelationshipRequest.h"
#include <vector>

class InsertModifiesRequest : public InsertRelationshipRequest {
public:
	InsertModifiesRequest(Rel::Modifies);

	void execute(RelationshipStorage& storage) override;

private:
	Rel::Modifies modifiesRel;
};