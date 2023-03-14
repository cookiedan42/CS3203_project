#pragma once

#include "./InsertPatternRequest.h"


class InsertAssignPatternRequest : public InsertPatternRequest {
public:
	InsertAssignPatternRequest(Ptn::AssignPtn);

	Ptn::AssignPtn getAssignPattern() const;

	void execute(PatternStorage&) const override;
private:
	Ptn::AssignPtn assignPattern;
};


