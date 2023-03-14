#pragma once
#include "../../Definitions/Definitions.h"

enum class OperatorSymbol {
	DEFAULT = 0,
	PLUS = 1,
	MINUS = 2,
	MULTIPLY = 3,
	DIVIDE = 4,
	MODULO = 5
};

class Operator {
private:

	const OperatorSymbol symbol;
	const int precedence;

public:
	Operator();
	Operator(OperatorSymbol symbol, int precedence);

	OperatorSymbol getSymbol() const;

	int comparePrecedence(Operator& other) const;
};

class PlusOperator : public Operator {
public:
	PlusOperator();
};

class MinusOperator : public Operator {
public:
	MinusOperator();
};

class MultiplyOperator : public Operator {
public:
	MultiplyOperator();
};

class DivideOperator : public Operator {
public:
	DivideOperator();
};

class ModuloOperator : public Operator {
public:
	ModuloOperator();
};