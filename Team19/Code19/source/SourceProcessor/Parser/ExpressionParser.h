#pragma once
#include <string>
#include <stack>
#include <unordered_map>
#include <memory>
#include "../../Definitions/Definitions.h"
#include "../AST/AST.h"
#include "../Tokenizer/TokenIterator.h"
#include "TerminalParser.h"
#include "Operator.h"
#include "ParseException.h"

using namespace std;

using namespace SP;

/**
 * @brief An expression parser that implements the Shunting Yard Algorithm.
 * 
 * References:
 * https://en.wikipedia.org/wiki/Shunting_yard_algorithm
 * https://www.klittlepage.com/2013/12/22/twelve-days-2013-shunting-yard-algorithm/
 */
class ExpressionParser {
private:
	unordered_map<string, Operator> operators;

	TokenIterator start;
	TokenIterator end;

	void buildSubTree(string op, stack<ExpressionNode>& stack);

	void parseRightParenthesis(stack<string>& operatorStack, stack<ExpressionNode>& operandStack);

	void parseBinaryOperator(string& symbol, stack<string>& operatorStack, stack<ExpressionNode>& operandStack);

	void parseRemainingOperators(stack<string>& operatorStack, stack<ExpressionNode>& operandStack);

public:
	ExpressionParser(TokenIterator start, TokenIterator end);

	ExpressionNode parse();
};
