#include "ExpressionParser.h"

ExpressionParser::ExpressionParser(TokenIterator start, TokenIterator end) : start(start), end(end) {
	operators.insert({ SIMPLE::PLUS_OPERATOR, PlusOperator() });
	operators.insert({ SIMPLE::MINUS_OPERATOR, MinusOperator() });
	operators.insert({ SIMPLE::MULTIPLY_OPERATOR, MultiplyOperator() });
	operators.insert({ SIMPLE::DIVIDE_OPERATOR, DivideOperator() });
	operators.insert({ SIMPLE::MODULO_OPERATOR, ModuloOperator() });
}

void ExpressionParser::buildSubTree(string op, stack<ExpressionNode>& stack) {
	ExpressionNode RHS = move(stack.top());
	stack.pop();

	ExpressionNode LHS = move(stack.top());
	stack.pop();

	auto binaryOperatorNode = make_unique<BinaryOperatorNode>(op, move(LHS), move(RHS));
	stack.push(move(binaryOperatorNode));
}

ExpressionNode ExpressionParser::parse() {
	stack<string> operatorStack;
	stack<ExpressionNode> operandStack;

	while (start != end) {
		if (start.isName()) {
			auto variable = TerminalParser::parseVariableName(start);
			operandStack.push(move(variable));
			continue;
		}
		if (start.isInteger()) {
			auto constant = TerminalParser::parseConstantValue(start);
			operandStack.push(move(constant));
			continue;
		}
		// Symbol token +, -, *, /, %, (, )
		string symbol = start.getValue();
		if (symbol == SIMPLE::LEFT_PARENTHESIS) {
			operatorStack.push(symbol);
			start.advance();
			continue;
		}
		if (symbol == SIMPLE::RIGHT_PARENTHESIS) {
			parseRightParenthesis(operatorStack, operandStack);
			continue;
		}
		parseBinaryOperator(symbol, operatorStack, operandStack);
	}
	parseRemainingOperators(operatorStack, operandStack);

	return move(operandStack.top());
}

void ExpressionParser::parseRightParenthesis(stack<string>& operatorStack, stack<ExpressionNode>& operandStack) {
	while (!operatorStack.empty() && operatorStack.top() != SIMPLE::LEFT_PARENTHESIS) {
		string op = operatorStack.top();
		operatorStack.pop();
		buildSubTree(op, operandStack);
	}
	if (operatorStack.empty()) throw ParseException(Exception::INVALID_SYNTAX_GRAMMAR);
	if (operatorStack.top() != SIMPLE::LEFT_PARENTHESIS) throw ParseException(Exception::INVALID_SYNTAX_GRAMMAR);

	operatorStack.pop();

	start.advance();
}

void ExpressionParser::parseBinaryOperator(
	string& symbol, stack<string>& operatorStack, stack<ExpressionNode>& operandStack) {
	if (!operators.count(symbol)) throw ParseException(Exception::INVALID_SYNTAX_GRAMMAR);

	Operator op = operators[symbol];
	while (!operatorStack.empty()) {
		if (operatorStack.top() == SIMPLE::LEFT_PARENTHESIS) break;

		Operator otherOp = operators[operatorStack.top()];
		if (op.comparePrecedence(otherOp) > 0) break;

		buildSubTree(operatorStack.top(), operandStack);
		operatorStack.pop();
	}
	operatorStack.push(symbol);

	start.advance();
}

void ExpressionParser::parseRemainingOperators(stack<string>& operatorStack, stack<ExpressionNode>& operandStack) {
	while (!operatorStack.empty()) {
		if (operatorStack.top() == SIMPLE::LEFT_PARENTHESIS) throw ParseException(Exception::INVALID_SYNTAX_GRAMMAR);

		buildSubTree(operatorStack.top(), operandStack);
		operatorStack.pop();
	}
}