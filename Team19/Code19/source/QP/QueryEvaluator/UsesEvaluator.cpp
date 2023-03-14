#include "UsesEvaluator.h"

UsesEvaluator::UsesEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

/**
* Evaluate clauses that have no synonyms.
*
* @param rel Uses design abstraction to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for the design abstraction
*/
bool UsesEvaluator::evaluateClauseWithNoSyn()
{
	RelationshipResults result = getRelationPairs();

	return !result.empty();

}

/**
* Get all relation pairs that satisfy the Uses relationship.
*
* @param rel Uses relationship to be evaluated
* @param facade Storage to be accessed
* @return pairs of entities that satisfy Uses relationship
*/
RelationshipResults UsesEvaluator::getRelationPairs()
{
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	RelationshipResults result;

	// Note: Arg1 of Uses cannot be WILDCARD only can be statement/procedure
	if (type == ArgsType::STMTNO_QUOTE)
	{
		// eg. Uses(1,"x")
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetUsesRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2, Arg1Type::STATEMENT);
		result = facade.get(request);
	}
	else if (type == ArgsType::STMTNO_WILDCARD || type == ArgsType::STMTNO_SYN)
	{
		// eg. Uses(1,_), Uses(1, v)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetUsesRequest>(arg1Value, Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
		result = facade.get(request);
	}
	else if (type == ArgsType::SYN_QUOTE)
	{
		// eg. Uses(s, "x"), Uses(p, "x"), this includes all statement and procedure synonyms (eg. statement, print, calls, while, if, assign, procedure)
		// (will be filtered in QueryEvaluator, when intersecting with declarations tables)
		Declaration dec1 = std::get<Declaration>(arg1.argumentVariant);
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;

		if (dec1.synType->type == DE::Procedure().type) {
			auto proc_request = std::make_shared<GetUsesRequest>(arg2Value, Mode::FILTER_ARG_2, Arg1Type::PROCEDURE);
			result = facade.get(proc_request);
		}
		else {
			auto stmt_request = std::make_shared<GetUsesRequest>(arg2Value, Mode::FILTER_ARG_2, Arg1Type::STATEMENT);
			result = facade.get(stmt_request);
		}

	}
	else if (type == ArgsType::SYN_SYN || type == ArgsType::SYN_WILDCARD)
	{
		// eg. Uses(s, v), Uses(s, _), Modifies(p, _), Modifies(p, v)
		Declaration dec1 = std::get<Declaration>(arg1.argumentVariant);
		if (dec1.synType->type == DE::Procedure().type) {
			auto request = std::make_shared<GetUsesRequest>(Mode::ALL, Arg1Type::PROCEDURE);
			result = facade.get(request);
		}
		else {
			auto request = std::make_shared<GetUsesRequest>(Mode::ALL, Arg1Type::STATEMENT);
			result = facade.get(request);
		}
	}
	else if (type == ArgsType::QUOTE_WILDCARD || type == ArgsType::QUOTE_SYN) {
		// eg.  Uses("proc_name", _), Uses("proc_name", v)
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetUsesRequest>(arg1Value, Mode::FILTER_ARG_1, Arg1Type::PROCEDURE);
		result = facade.get(request);
	}
	else if (type == ArgsType::QUOTE_QUOTE) {
		// eg. Uses("proc_name", "x")
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetUsesRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2, Arg1Type::PROCEDURE);
		result = facade.get(request);
	}
	return result;
}
