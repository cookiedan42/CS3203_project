#include "PatternAssignEvaluator.h"

/**
* Evaluate clauses that have no common synonym with other clauses.
*
* @param pt Pattern clause to be evaluated
* @param pkb Storage to be accessed
* @return boolean result if values exist for evaluated pattern clause
*/
bool PatternAssignEvaluator::evaluateNoCommSyn(shared_ptr<PatternClause> pt, GetFacade* pkb)
{
	PatternResults result = getAllMatches(pt, pkb);

	return !result.empty();

}

/**
* Get all pairs of assignment stmt_no and variable modified that match the pattern.
*
* @param pt Assignment pattern clause to be evaluated
* @param pkb Storage to be accessed
* @return pairs of assignment stmt_no and variable modified that match the pattern.
*/
PatternResults PatternAssignEvaluator::getAllMatches(shared_ptr<PatternClause> pt, GetFacade* pkb)
{
	Argument arg1 = pt->arg_1;
	Argument arg2 = pt->arg_2;
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);

	std::shared_ptr<GetPatternRequest> request;
	PatternResults result;

	if (type == ArgsType::SYN_WILDCARD || type == ArgsType::WILDCARD_WILDCARD)
	{
		// eg. pattern a (v, _), pattern a (_,_) --> getAll
		request = std::make_shared<GetAssignPatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
	}
	if (type == ArgsType::SYN_QUOTE || type == ArgsType::WILDCARD_QUOTE)
	{
		// eg. pattern a (v, "x"), pattern a (_, "x") --> get by arg2, full exp
		ArgString completeArg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		request = std::make_shared <GetAssignPatternRequest>(PatternMode::COMPLETE_ARG2, EMPTY_ARG, completeArg2Value);
	}
	if (type == ArgsType::SYN_SUBQUOTE || type == ArgsType::WILDCARD_SUBQUOTE)
	{
		// eg. pattern a (v, _"x"_), pattern a (_, _"x"_) --> get by arg2, sub exp

		ArgString subArg2Value = std::get<SubExp>(arg2.argumentVariant).value;
		request = std::make_shared <GetAssignPatternRequest>(PatternMode::SUB_ARG2, EMPTY_ARG, subArg2Value);
	}
	if (type == ArgsType::QUOTE_WILDCARD)
	{
		// eg. pattern a ("y", _) --> get by arg1
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		request = std::make_shared <GetAssignPatternRequest>(PatternMode::ARG1, arg1Value, EMPTY_ARG);
	}
	if (type == ArgsType::QUOTE_QUOTE)
	{
		// eg. pattern a ("y", "x") --> get by arg1, arg2, full exp

		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		ArgString completeArg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		request = std::make_shared <GetAssignPatternRequest>(PatternMode::ARG1_COMPLETE_ARG2, arg1Value, completeArg2Value);
	}
	if (type == ArgsType::QUOTE_SUBQUOTE)
	{
		// eg. pattern a ("y", _"x"_) --> get by arg1, arg2, sub exp
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		ArgString subArg2Value = std::get<SubExp>(arg2.argumentVariant).value;
		request = std::make_shared <GetAssignPatternRequest>(PatternMode::ARG1_SUB_ARG2, arg1Value, subArg2Value);
	}
	result = pkb->get(request);
	return result;
}


/**
* Get table of synonym values that match the assignment pattern.
*
* @param pt Assignment pattern clause to be evaluated
* @param pkb Storage to be accessed
* @return table of synonym values that match the assignment pattern
*/
Table PatternAssignEvaluator::evaluateHasCommSyn(shared_ptr<PatternClause> pt, GetFacade* pkb)
{
	Argument arg1 = pt->arg_1;
	Argument arg2 = pt->arg_2;
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	PatternResults result = getAllMatches(pt, pkb);
	Table table;

	// Note: even if result is empty, still create a table with appropriate header to find intersection
	if (type == ArgsType::SYN_WILDCARD || type == ArgsType::SYN_QUOTE || type == ArgsType::SYN_SUBQUOTE)
	{
		// eg. pattern a (v, _), pattern a (v, "x"), pattern a (v, _"x"_)
		Declaration arg1Dec = std::get<Declaration>(arg1.argumentVariant);
		table = QueryUtil::createTableArg1Arg2(pt->synonym.synName, arg1Dec.synName, result);
	}
	else {
		table = QueryUtil::createTableArg1(pt->synonym.synName, result);
	}
	return table;

}