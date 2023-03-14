#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "../../Definitions/Definitions.h"
#include "../Tokenizer/TokenIterator.h"
#include "../Tokenizer/Token.h"
#include "../Validator/ValidatorUtil.h"

using validation_error = std::invalid_argument;

class ExpValidator {
	TokenIterator start;
	TokenIterator end;
	int validateExpressionRec(int depth);

public:
	ExpValidator(TokenIterator start, TokenIterator end);

	void validateExpression();
};