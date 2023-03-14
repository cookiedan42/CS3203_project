#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "./GetRelationshipRequest.h"

class GetCallsRequest : public GetRelationshipRequest {
public:
	GetCallsRequest(Arg arg1, Arg arg2, Mode mode, HasStar star = NO_STAR);
	GetCallsRequest(Arg arg, Mode mode, HasStar star = NO_STAR);
	GetCallsRequest(Mode mode, HasStar star = NO_STAR);

	bool getHasStar() const;

	RelationshipResults execute(RelationshipStorage& storage) override;

private:
	HasStar hasStar;
};