#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "./GetRelationshipRequest.h"

class GetNextRequest : public GetRelationshipRequest {
public:
	GetNextRequest(Arg arg1, Arg arg2, Mode mode, HasStar star = NO_STAR);
	GetNextRequest(Arg arg, Mode mode, HasStar star = NO_STAR);
	GetNextRequest(Mode mode, HasStar star = NO_STAR);

	bool getHasStar() const;
	
	RelationshipResults execute(RelationshipStorage& storage) override;

private:
	HasStar hasStar;
};