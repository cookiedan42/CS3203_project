#pragma once
#include <string>
#include <memory>
#include "../../Definitions/Definitions.h"
#include "../AST/AST.h"
#include "../Tokenizer/TokenIterator.h"
#include "ExpressionParser.h"
#include "ParserUtil.h"

using namespace std;
using namespace SP;

class ConditionalExpressionParser {
public:
	ConditionalExpressionParser(TokenIterator head);

	const TokenIterator& getIterator() const;

	unique_ptr<ConditionalExpressionNode> parseConditionalExpression();

private:
	TokenIterator head;

	unique_ptr<NotConditionalExpressionNode> parseNotConditionalExpression();

	unique_ptr<ConditionalExpressionNode> parseAndOrConditionalExpression();

	unique_ptr<RelationalExpressionNode> parseRelationalExpression();

	RelationalFactorNode parseRelationalFactor(TokenIterator start, TokenIterator end);
};