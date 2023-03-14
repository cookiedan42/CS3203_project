#pragma once

#include "./InsertPatternRequest.h"

class InsertWhilePatternRequest : public InsertPatternRequest {
public:
	InsertWhilePatternRequest(Ptn::WhilePtn);

	Ptn::WhilePtn getWhilePattern() const;

	void execute(PatternStorage&) const override;
private:
	Ptn::WhilePtn whilePattern;
};


