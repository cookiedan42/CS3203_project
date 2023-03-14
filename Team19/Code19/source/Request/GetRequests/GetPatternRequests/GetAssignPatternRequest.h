#pragma once
#include "../../../SourceProcessor/DesignExtractor/DesignExtractorUtil.h"
#include "../../../SourceProcessor/Parser/ExpressionParser.h"
#include "../../../SourceProcessor/Tokenizer/SPTokenizer.h"
#include "./GetPatternRequest.h"

class GetAssignPatternRequest : public GetPatternRequest {
public:
	GetAssignPatternRequest(PatternMode mode, Arg arg1, Arg arg2);
	PatternResults execute(PatternStorage&) const override;
};
