#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "./GetRelationshipRequest.h"

class GetParentRequest : public GetRelationshipRequest {
public:
	GetParentRequest(Arg arg1, Arg arg2, Mode mode, HasStar star = NO_STAR);
	GetParentRequest(Arg arg, Mode mode, HasStar star = NO_STAR);
	GetParentRequest(Mode mode, HasStar star = NO_STAR);

	bool getHasStar() const;

	RelationshipResults execute(RelationshipStorage& storage) override;

private:
	HasStar hasStar;
};