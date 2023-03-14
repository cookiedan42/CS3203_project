#include "./Validator.h"

void Validator::assertEq(std::string target) {
	if (!head.valueIs(target)) {
		throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
	}
}

void Validator::assertIsInt() {
	if (!head.isInteger()) {
		throw validation_error(Exception::INVALID_INTEGER);
	}
}

void Validator::assertIsName() {
	if (!head.isName()) {
		throw validation_error(Exception::INVALID_NAME);
	}
}

Validator::Validator(std::vector<Token> tokens) : tokens(tokens) {}

void Validator::validateProgram() {
	if (tokens.size() == 0) { throw validation_error(Exception::EMPTY_SOURCE); }
	
	while (head != end && head.valueIs(SIMPLE::PROCEDURE)) {
		//head is pointing at first procedure
		validateProcedure();
	}
	if (head != end) {
		throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
	}
	if (!callValidator.isValid()) {
		throw validation_error(Exception::SEMANTIC_ERROR);
	}
}


void Validator::validateProcedure() {
	// procedure name { <statement list> }
	
	// isIn proc as part of Callvalidator?
	if (isInProc) { throw validation_error(Exception::SEMANTIC_ERROR); }
	isInProc = true;

	assertEq(SIMPLE::PROCEDURE);
	head.advance();
	assertIsName();
	callValidator.setProc(head.getValue());
	head.advance();
	validateStatementList();
	head.advance();
	isInProc = false;
	// head is now pointing to start of next procedure
}


void Validator::validateStatementList() {
	// from { to }
	assertEq(SIMPLE::LEFT_CURLY);
	head.advance();
	if (head.valueIs(SIMPLE::RIGHT_CURLY)) { throw validation_error(Exception::EMPTY_STATEMENT_LIST); }
	while (!head.valueIs(SIMPLE::RIGHT_CURLY)) {
		validateStatement();
	}
	assertEq(SIMPLE::RIGHT_CURLY);
}

/* 
	Decide which type of statement, then redirects accordingly
	each statement validator ends with the Iterator pointing to:
	either	the first token of next statement in the list
	or		the closing curly of the list
*/
void Validator::validateStatement() {

	if (head.valueIs(SIMPLE::EQUALS,1) ) {
		validateAssign();
	} 
	else if (head.valueIs(SIMPLE::READ)) {
		validateRead();
	}
	else if (head.valueIs(SIMPLE::PRINT)) {
		validatePrint();
	} 
	else if (head.valueIs(SIMPLE::IF) ){
		validateIf();
	} 
	else if (head.valueIs(SIMPLE::WHILE)) {
		validateWhile();
	} 
	else if (head.valueIs(SIMPLE::CALL)) {
		validateCall();
	} 
	else {
		throw validation_error(Exception::INVALID_SYNTAX_GRAMMAR);
	}
}

void Validator::validateIf() {
	assertEq(SIMPLE::IF);
	head.advance();

	assertEq(SIMPLE::LEFT_BRACKET);
	head.advance();
	auto start_of_conditon = head;	// point after opening bracket
	head.advanceTo(SIMPLE::LEFT_CURLY);
	head.back(); // then 
	head.back(); // )
	assertEq(SIMPLE::RIGHT_BRACKET);
	auto end_of_conditon = head;	// point at closing bracket
	CondValidator(start_of_conditon,end_of_conditon).validate();
	head.advance();

	assertEq(SIMPLE::THEN);
	head.advance();
	validateStatementList();
	head.advance();

	assertEq(SIMPLE::ELSE);
	head.advance();
	validateStatementList();
	head.advance();

}

void Validator::validateWhile() {
	assertEq(SIMPLE::WHILE);
	head.advance();

	assertEq(SIMPLE::LEFT_BRACKET);
	head.advance();
	auto start_of_conditon = head;
	head.advanceTo(SIMPLE::LEFT_CURLY);
	head.back();
	assertEq(SIMPLE::RIGHT_BRACKET);
	auto end_of_conditon = head;
	head.advance();
	CondValidator(start_of_conditon, end_of_conditon).validate();
	
	validateStatementList();
	head.advance();
}

void Validator::validateRead() {
	assertEq(SIMPLE::READ);
	head.advance();

	assertIsName();
	head.advance();
	
	assertEq(SIMPLE::EOL);
	head.advance();
}

void Validator::validatePrint() {
	assertEq(SIMPLE::PRINT);
	head.advance();

	assertIsName();
	head.advance();
	
	assertEq(SIMPLE::EOL);
	head.advance();
}

void Validator::validateCall() {
	assertEq(SIMPLE::CALL);
	head.advance();
	
	assertIsName();
	callValidator.addCall(head.getValue());
	head.advance();

	assertEq(SIMPLE::EOL);
	head.advance();
}

void Validator::validateAssign() {
	// LHS_name = RHS_Expression
	assertIsName();
	head.advance();

	assertEq(SIMPLE::EQUALS);
	head.advance();

	auto start_of_exp = head;
	head.advanceTo(SIMPLE::EOL);
	auto end_of_exp = head;
	ExpValidator(start_of_exp, end_of_exp).validateExpression();
	head.advance();
}
