#pragma once
#include <vector>
#include <algorithm>
#include "../../Definitions/Definitions.h"
#include "../Tokenizer/SPTokenizer.h"
#include "../Tokenizer/TokenIterator.h"
#include "./ExpValidator.h"

/*
Validates conditional expression
ie between the brackets of if() and while()
*/
class CondValidator {
	TokenIterator start;
	TokenIterator end;

	bool testIsCompToken(Token token);
	bool testIsBoolToken(Token token);

	void condRec(TokenIterator start, TokenIterator end);
	void rel(TokenIterator start, TokenIterator end);

public:
	CondValidator(TokenIterator start, TokenIterator end);

	void validate();
};