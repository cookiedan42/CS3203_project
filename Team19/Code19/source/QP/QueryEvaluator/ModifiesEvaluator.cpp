#include "ModifiesEvaluator.h"

ModifiesEvaluator::ModifiesEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

/**
* Evaluate clauses that have no synonyms.
*
* @param rel Modifies design abstraction to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for the design abstraction
*/
bool ModifiesEvaluator::evaluateClauseWithNoSyn()
{
	RelationshipResults result = getRelationPairs();
	
	return !result.empty();

}

/**
* Get all relation pairs that satisfy the Modifies relationship.
*
* @param rel Modifies relationship to be evaluated
* @param facade Storage to be accessed
* @return pairs of entities that satisfy Modifies relationship
*/
RelationshipResults ModifiesEvaluator::getRelationPairs()
{
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	RelationshipResults result;

	// Note: Arg1 of Modifies cannot be WILDCARD, only can be procedure/statement 
	if (type == ArgsType::STMTNO_QUOTE)
	{
		// eg. Modifies(1,"x")
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetModifiesRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2, Arg1Type::STATEMENT);
		result = facade.get(request);
	}
	else if (type == ArgsType::STMTNO_WILDCARD || type == ArgsType::STMTNO_SYN)
	{
		// eg. Modifies(1,_), Modifies(1, v)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetModifiesRequest>(arg1Value, Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
		result = facade.get(request);
	}
	else if (type == ArgsType::SYN_QUOTE)
	{
		// eg. statement s; procedure p; Modifies(s, "x"), Modifies(p, "x")
		Declaration dec1 = std::get<Declaration>(arg1.argumentVariant);
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		if (dec1.synType->type == DE::Procedure().type) {
			auto proc_request = std::make_shared<GetModifiesRequest>(arg2Value, Mode::FILTER_ARG_2, Arg1Type::PROCEDURE);
			result = facade.get(proc_request);
		}
		else {
			auto stmt_request = std::make_shared<GetModifiesRequest>(arg2Value, Mode::FILTER_ARG_2, Arg1Type::STATEMENT);
			result = facade.get(stmt_request);
		}
	}
	else if (type == ArgsType::SYN_SYN || type == ArgsType::SYN_WILDCARD)
	{
		// eg. Modifies(s, v), Modifies(s, _), Modifies(p, _), Modifies(p, v)
		Declaration dec1 = std::get<Declaration>(arg1.argumentVariant);
		if (dec1.synType->type == DE::Procedure().type) {
			auto request = std::make_shared<GetModifiesRequest>(Mode::ALL, Arg1Type::PROCEDURE);
			result = facade.get(request);
		}
		else {
			auto request = std::make_shared<GetModifiesRequest>(Mode::ALL, Arg1Type::STATEMENT);
			result = facade.get(request);
		}
	}
	else if (type == ArgsType::QUOTE_WILDCARD || type == ArgsType::QUOTE_SYN) {
		// eg.  Modifies("proc_name", _), Modifies("proc_name", v)
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetModifiesRequest>(arg1Value, Mode::FILTER_ARG_1, Arg1Type::PROCEDURE);
		result = facade.get(request);
	}
	else if (type == ArgsType::QUOTE_QUOTE) {
		// eg. Modifies("proc_name", "x")
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetModifiesRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2, Arg1Type::PROCEDURE);
		result = facade.get(request);
	}
	return result;
}
