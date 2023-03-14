#include "./CondValidator.h"

CondValidator::CondValidator(TokenIterator start, TokenIterator end) : start(start), end(end) {};

void CondValidator::validate() {
	// tokens are at least a<b
	if (end-start < 2) { throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR); }
	condRec(start, end);
}

void CondValidator::condRec(TokenIterator start, TokenIterator end) {
	
	// remove not clause and recurse
	if (start.valueIs(SIMPLE::NOT)) {
		if (!start.valueIs(SIMPLE::LEFT_BRACKET, 1) || !end.valueIs(SIMPLE::RIGHT_BRACKET , -1)) {
			throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
		}
		condRec(start + 2, end - 1);
		return;
	}

	// no brackets means it's not boolean
	if (!start.valueIs(SIMPLE::LEFT_BRACKET) ) {
		rel(start,end);
		return;
	}

	// find matching bracket for first half
	auto start_first = start;
	auto end_first = start+1;
	int brackets = 1;
	while (brackets > 0) {
		if (end_first.valueIs(SIMPLE::LEFT_BRACKET)) { brackets++; }
		if (end_first.valueIs(SIMPLE::RIGHT_BRACKET)) { brackets--; }
		end_first.advance();
		if (end_first == end) { 
			throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR); }
	}

	// end_first is pointing past the matched bracket

	if (end_first.isComparator()) {
		rel(start, end);
		return;
	}

	if (end_first.isBoolean()) {
		// check that back half is bracketed too
		if (!end_first.valueIs(SIMPLE::LEFT_BRACKET,1) || !end.valueIs(SIMPLE::RIGHT_BRACKET, -1) ) {
			throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
		}
		condRec(start + 1, end_first-1);
		condRec(end_first+2, end - 1)	;
		return;
	} 
	throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
}

void CondValidator::rel(TokenIterator start, TokenIterator end) {
	// split on comp
	// send each half to expr

	auto mid = start;
	while (mid != end) {
		if (mid.isComparator()) {
			ExpValidator(start, mid).validateExpression();
			ExpValidator((mid + 1), end).validateExpression();
			return;
		}
		mid.advance();
	}
	throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
}