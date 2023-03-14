#pragma once

#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "../../../Relationship/Relationship.h"
#include "./GetRelationshipRequest.h"

class GetUsesRequest : public GetRelationshipRequest {
public:
	GetUsesRequest(Arg arg1, Arg arg2, Mode mode, Arg1Type arg1Type = Arg1Type::ANY);
	GetUsesRequest(Arg arg, Mode mode, Arg1Type arg1Type = Arg1Type::ANY);
	GetUsesRequest(Mode mode, Arg1Type arg1Type = Arg1Type::ANY);

	bool getHasArg1Type() const;
	Arg1Type getArg1Type() const;

	RelationshipResults execute(RelationshipStorage& storage) override;

private:
	bool hasArg1Type;
	Arg1Type arg1Type;
};