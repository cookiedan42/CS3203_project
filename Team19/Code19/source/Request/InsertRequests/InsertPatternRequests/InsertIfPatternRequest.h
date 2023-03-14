#pragma once

#include "./InsertPatternRequest.h"


class InsertIfPatternRequest : public InsertPatternRequest {
public:
	InsertIfPatternRequest(Ptn::IfPtn);

	Ptn::IfPtn getIfPattern() const;

	void execute(PatternStorage&) const override;
private:
	Ptn::IfPtn ifPattern;
};


