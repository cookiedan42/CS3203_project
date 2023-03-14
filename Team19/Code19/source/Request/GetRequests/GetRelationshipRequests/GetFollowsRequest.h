#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "./GetRelationshipRequest.h"

class GetFollowsRequest : public GetRelationshipRequest {
public:
	GetFollowsRequest(Arg arg1, Arg arg2, Mode mode, HasStar star = NO_STAR);
	GetFollowsRequest(Arg arg, Mode mode, HasStar star = NO_STAR);
	GetFollowsRequest(Mode mode, HasStar star = NO_STAR);

	bool getHasStar() const;

	RelationshipResults execute(RelationshipStorage& storage) override;

private:
	HasStar hasStar;
};