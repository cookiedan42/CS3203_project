#include "RelationshipEvaluator.h"

RelationshipEvaluator::RelationshipEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	arg1(arg1), arg2(arg2), facade(facade) {}

/**
* Evaluate clause that have no common synonym with other clauses.
*
* @return true if values exist for the design abstraction, and false otherwise
*/
bool RelationshipEvaluator::evaluateClauseWithSynToBool() {
	RelationshipResults result = getRelationPairs();

	return !result.empty();
}

/**
* Get table of synonym values that satisfy the design abstraction.
*
* @return table of synonym values that satisfy the design abstraction
*/
Table RelationshipEvaluator::evaluateClauseWithSyn() {
	ArgType arg1Type = QueryUtil::getArgTypeFromArgument(arg1);
	ArgType arg2Type = QueryUtil::getArgTypeFromArgument(arg2);

	RelationshipResults result = getRelationPairs();
	Table table;

	bool isArg1Syn = arg1Type == ArgType::SYN && arg2Type != ArgType::SYN;
	bool isArg2Syn = arg2Type == ArgType::SYN && arg1Type != ArgType::SYN;

	// Note: even if result is empty, still create a table with appropriate header
	if (isArg1Syn) { // eg. Affects(s, 1), Affects(s, _)
		Declaration dec1 = std::get<Declaration>(arg1.argumentVariant);
		if (dec1.synType->type == DE::Procedure().type) {
			table = QueryUtil::createTableArg1(dec1.synName, result);
		}
		else {
			table = createTableOnArg1StatementType(dec1, facade, result);
		}
	}
	else if (isArg2Syn) { // eg. Affects(1, s), Affects(_, s)
		Declaration dec2 = std::get<Declaration>(arg2.argumentVariant);
		if (dec2.synType->type == DE::Procedure().type) {
			table = QueryUtil::createTableArg2(dec2.synName, result);
		}
		else {
			table = createTableOnArg2StatementType(dec2, facade, result);
		}
	}
	else { // Both arg are syn eg. Affects(s1,s2)
		Declaration dec1 = std::get<Declaration>(arg1.argumentVariant);
		Declaration dec2 = std::get<Declaration>(arg2.argumentVariant);
		if (dec1.synType->type == DE::Procedure().type) {
			table = QueryUtil::createTableArg1Arg2(dec1.synName, dec2.synName, result);
		}
		else {
			table = createTableOnArg1Arg2StatementType(dec1, dec2, facade, result);
		}
	}

	return table;
}

Table RelationshipEvaluator::createTableOnArg1StatementType(Declaration dec, GetFacade& facade, RelationshipResults resultPairs)
{
	Table table;
	// check statement type to reduce table size
	if (dec.synType->type == STATEMENT_DE) {
		table = QueryUtil::createTableArg1(dec.synName, resultPairs);
	}
	else {
		SynonymResults statementResults = dec.synType->getSynonymFromPKB(&facade);
		table = QueryUtil::createTableArg1ForStatementType(dec.synName, resultPairs, statementResults);
	}
	return table;
}

Table RelationshipEvaluator::createTableOnArg2StatementType(Declaration dec, GetFacade& facade, RelationshipResults resultPairs)
{
	Table table;
	// check statement type to reduce table size
	if (dec.synType->type == STATEMENT_DE) {
		table = QueryUtil::createTableArg2(dec.synName, resultPairs);
	}
	else {
		SynonymResults statementResults = dec.synType->getSynonymFromPKB(&facade);
		table = QueryUtil::createTableArg2ForStatementType(dec.synName, resultPairs, statementResults);
	}
	return table;
}

Table RelationshipEvaluator::createTableOnArg1Arg2StatementType(Declaration dec1, Declaration dec2, GetFacade& facade, RelationshipResults resultPairs)
{
	Table table;
	// check statement type to reduce table size
	if (!QueryUtil::isStatementType(dec1) && !QueryUtil::isStatementType(dec2)) {
		table = QueryUtil::createTableArg1Arg2(dec1.synName, dec2.synName, resultPairs);
	}
	else {
		SynonymResults arg1Results = dec1.synType->getSynonymFromPKB(&facade);
		SynonymResults arg2Results = dec2.synType->getSynonymFromPKB(&facade);
		if (dec1.synType->type == STATEMENT_DE && QueryUtil::isStatementType(dec2)) {
			table = QueryUtil::createTableArg1Arg2ForStatementType(dec1.synName, dec2.synName, resultPairs, arg1Results, arg2Results, false, true);
		}
		else if (dec2.synType->type == STATEMENT_DE && QueryUtil::isStatementType(dec1)) {
			table = QueryUtil::createTableArg1Arg2ForStatementType(dec1.synName, dec2.synName, resultPairs, arg1Results, arg2Results, true, false);
		}
		else {
			table = QueryUtil::createTableArg1Arg2ForStatementType(dec1.synName, dec2.synName, resultPairs, arg1Results, arg2Results, true, true);
		}
	}
	return table;
}