#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../../Definitions/Definitions.h"
#include "../AST/AST.h"
#include "../Tokenizer/TokenIterator.h"
#include "ConditionalExpressionParser.h"
#include "TerminalParser.h"
#include "ParserUtil.h"
#include "ParseException.h"

using namespace std;
using namespace SP;

class StatementParser {
public:
	StatementParser(TokenIterator head, int statementNo);

	const TokenIterator& getIterator() const;

	const int& getStatementNo() const;

	vector<StatementNode> parseStatementList();
private:
	TokenIterator head;

	int statementNo;

	StatementNode parseStatement();

	unique_ptr<AssignStatementNode> parseAssignStatement();

	unique_ptr<ReadStatementNode> parseReadStatement();

	unique_ptr<PrintStatementNode> parsePrintStatement();

	unique_ptr<CallStatementNode> parseCallStatement();

	unique_ptr<WhileStatementNode> parseWhileStatement();

	unique_ptr<IfStatementNode> parseIfStatement();
};