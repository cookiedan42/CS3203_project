#pragma once

#include "../../Request.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "../../../PKB/Storage/PatternStorage.h"

enum PatternMode {
	NO_ARG,
	SUB_ARG2,
	COMPLETE_ARG2,
	ARG1,
	ARG1_SUB_ARG2,
	ARG1_COMPLETE_ARG2
};

enum PatternType {
	ASSIGNMENT_PATTERN,
	IF_PATTERN,
	WHILE_PATTERN
};


class GetPatternRequest : public Request {
public:
	GetPatternRequest(PatternType type, PatternMode mode, Arg arg1, Arg arg2);
	GetPatternRequest() = default;
	PatternType getType() const;
	PatternMode getMode() const;
	Arg getArg1() const;
	Arg getArg2() const;
	bool getHasArg1() const;
	bool getHasArg2() const;
	virtual PatternResults execute(PatternStorage&) const = 0;
	

private:
	PatternType type;
	PatternMode mode;
	Arg arg1;
	Arg arg2;
	bool hasArg1;
	bool hasArg2;
};
