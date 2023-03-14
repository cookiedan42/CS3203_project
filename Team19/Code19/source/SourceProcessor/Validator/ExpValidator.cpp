#include "ExpValidator.h"

ExpValidator::ExpValidator(TokenIterator start, TokenIterator end) : start(start), end(end) {}

void ExpValidator::validateExpression() {
	int depth = validateExpressionRec(0);
	if (start != end || depth !=0) { throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR); }
	// if return to this caller and start != end,
	// means that more close than open brackets
}

int ExpValidator::validateExpressionRec(int depth) {

	bool should_be_symbol = false;
	while (true) {
		if (start == end) { throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR); }
		if (should_be_symbol) {
			// can only be close bracket, no open bracket
			if (start.valueIs(SIMPLE::RIGHT_BRACKET)) {
				start.advance();
				return depth-1;
			}
			if (!start.isMathOperator()) { throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR); }
			start.advance();

		} else {
			// can only be open bracket, no close bracket
			if (start.valueIs(SIMPLE::LEFT_BRACKET)) {
				start.advance();
				validateExpressionRec(depth+1);
			} else if (start.isInteger() || start.isName()) {
				start.advance();
			} else {
				throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
			}
			// if the overall expression ends on factor, it's valid
			if (start == end && depth == 0) { return depth; }
		}
		should_be_symbol = !should_be_symbol;
	}
}