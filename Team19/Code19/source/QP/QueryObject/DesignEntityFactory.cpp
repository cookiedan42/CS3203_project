#include "DesignEntityFactory.h"

/*
* Create design entity object.
*
* Note: If-Else is needed to create the relevant design entity object.
*/
shared_ptr<DesignEntity> DesignEntityFactory::createDesignEntity(std::string synTypeStr)
{
	if (synTypeStr == STATEMENT_DE) {
		return make_shared<DE::Statement>();
	}
	else if (synTypeStr == PROCEDURE_DE) {
		return make_shared<DE::Procedure>();
	}
	else if (synTypeStr == READ_DE) {
		return make_shared<DE::Read>();
	}
	else if (synTypeStr == PRINT_DE) {
		return make_shared<DE::Print>();
	}
	else if (synTypeStr == ASSIGN_DE) {
		return make_shared<DE::Assign>();
	}
	else if (synTypeStr == CALL_DE) {
		return make_shared<DE::Call>();
	}
	else if (synTypeStr == WHILE_DE) {
		return make_shared<DE::While>();
	}
	else if (synTypeStr == IF_DE) {
		return make_shared<DE::If>();
	}
	else if (synTypeStr == VARIABLE_DE) {
		return make_shared<DE::Variable>();
	}
	else if (synTypeStr == CONSTANT_DE) {
		return make_shared<DE::Constant>();
	}
	else {
		throw QuerySyntaxException("Design entity of '" + synTypeStr + "' not valid");
	}
}