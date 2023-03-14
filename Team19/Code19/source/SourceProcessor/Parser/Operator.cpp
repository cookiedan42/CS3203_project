#include "Operator.h"

Operator::Operator() : symbol(OperatorSymbol::DEFAULT), precedence(SIMPLE::DEFAULT_PRECEDENCE) {}

Operator::Operator(OperatorSymbol symbol, int precedence) : symbol(symbol), precedence(precedence) {}

OperatorSymbol Operator::getSymbol() const {
	return symbol;
}

int Operator::comparePrecedence(Operator& other) const {
	return precedence > other.precedence
		? 1 
		: other.precedence == precedence
			? 0
			: -1;
}

PlusOperator::PlusOperator() : Operator(OperatorSymbol::PLUS, SIMPLE::PLUS_PRECEDENCE) {}

MinusOperator::MinusOperator() : Operator(OperatorSymbol::MINUS, SIMPLE::MINUS_PRECEDENCE) {}

MultiplyOperator::MultiplyOperator() : Operator(OperatorSymbol::MULTIPLY, SIMPLE::MULTIPLY_PRECEDENCE) {}

DivideOperator::DivideOperator() : Operator(OperatorSymbol::DIVIDE, SIMPLE::DIVIDE_PRECEDENCE) {}

ModuloOperator::ModuloOperator() : Operator(OperatorSymbol::MODULO, SIMPLE::MODULO_PRECEDENCE) {}