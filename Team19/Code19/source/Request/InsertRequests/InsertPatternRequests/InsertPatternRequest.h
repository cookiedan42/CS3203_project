#pragma once

#include "../../../Definitions/Definitions.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../../Request.h"
#include "../../../PKB/Storage/PatternStorage.h"



class InsertPatternRequest : public Request {
public:
	InsertPatternRequest() {}

	virtual void execute(PatternStorage&) const = 0;

	~InsertPatternRequest() = default;
};

