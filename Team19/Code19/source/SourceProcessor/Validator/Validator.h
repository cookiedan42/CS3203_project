#pragma once
#include <stdexcept>
#include <algorithm>
#include <regex>
#include "../../Definitions/Definitions.h"
#include "../Tokenizer/Token.h"
#include "./ExpValidator.h"
#include "./CondValidator.h"
#include "./CallValidator.h"
#include "./ValidatorUtil.h"
#include "../Tokenizer/TokenIterator.h"

using validation_error = std::invalid_argument;

class Validator {
	std::vector<Token> tokens;
	TokenIterator head = TokenIterator::cbegin(tokens);
	TokenIterator end = TokenIterator::cend(tokens);

	bool isInProc = false;
	CallValidator callValidator = {};

	void assertEq(std::string target);
	void assertIsInt();
	void assertIsName();

public:
	Validator(std::vector<Token> tokens);

	void validateProgram();
	void validateProcedure();
	void validateStatementList();
	void validateStatement();
	void validateIf();
	void validateWhile();
	void validateRead();
	void validatePrint();
	void validateAssign();
	void validateCall();
};
