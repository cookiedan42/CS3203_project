#pragma once

#include "./GetPatternRequest.h"

class GetIfPatternRequest : public GetPatternRequest {
public:
	GetIfPatternRequest(PatternMode mode, Arg arg1, Arg arg2);
	PatternResults execute(PatternStorage&) const override;
};
