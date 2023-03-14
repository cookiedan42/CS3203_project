#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../../Definitions/Definitions.h"
#include "../AST/AST.h"
#include "../Tokenizer/Token.h"
#include "../Tokenizer/TokenIterator.h"
#include "StatementParser.h"
#include "ParserUtil.h"
#include "ParseException.h"

using namespace std;

using namespace SP;

class Parser {
private:
	vector<Token> tokens;
	TokenIterator head = TokenIterator::cbegin(tokens);
	TokenIterator end = TokenIterator::cend(tokens);

	int statementNo;

	unique_ptr<ProgramNode> parseProgram();

	unique_ptr<ProcedureNode> parseProcedure();

public:
	Parser(vector<Token> tokens);

	unique_ptr<ProgramNode> parseTokens();
};
