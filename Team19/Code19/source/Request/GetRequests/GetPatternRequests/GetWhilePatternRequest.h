#pragma once

#include "./GetPatternRequest.h"

class GetWhilePatternRequest : public GetPatternRequest {
public:
	GetWhilePatternRequest(PatternMode mode, Arg arg1, Arg arg2);
	PatternResults execute(PatternStorage&) const override;
};


