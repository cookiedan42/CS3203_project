#pragma once
#include "../../../PKB/Storage/RelationshipStorage.h"
#include "../../Request.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"

enum class Mode {
	ALL,
	FILTER_ARG_1,
	FILTER_ARG_2,
	FILTER_ARG1_ARG2
};

class GetRelationshipRequest : public Request {
public:
	GetRelationshipRequest(Arg arg1, Arg arg2, Mode mode);
	GetRelationshipRequest(Arg arg, Mode mode);
	GetRelationshipRequest(Mode mode);

	Mode getMode() const;
	Arg getArg1() const;
	Arg getArg2() const;

	virtual RelationshipResults execute(RelationshipStorage& storage);

protected:
	Mode mode; //The type of Get API
	Arg arg1;
	Arg arg2;
};