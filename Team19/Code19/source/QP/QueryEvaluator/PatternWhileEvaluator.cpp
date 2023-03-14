#include "PatternWhileEvaluator.h"

/**
* Evaluate clauses that have no common synonym with other clauses.
*
* @param pt Pattern clause to be evaluated
* @param pkb Storage to be accessed
* @return boolean result if values exist for evaluated pattern clause
*/
bool PatternWhileEvaluator::evaluateNoCommSyn(shared_ptr<PatternClause> pt, GetFacade* pkb)
{
	PatternResults result = getAllMatches(pt, pkb);

	return !result.empty();

}

/**
* Get all pairs of while stmt_no and variable used that match the pattern.
*
* @param pt While pattern clause to be evaluated
* @param pkb Storage to be accessed
* @return pairs of while stmt_no and variable used that match the pattern.
*/
PatternResults PatternWhileEvaluator::getAllMatches(shared_ptr<PatternClause> pt, GetFacade* pkb)
{
	Argument arg1 = pt->arg_1;
	Argument arg2 = pt->arg_2;
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	PatternResults result;

	if (type == ArgsType::SYN_WILDCARD || type == ArgsType::WILDCARD_WILDCARD)
	{
		// eg. pattern w (v, _), pattern w (_,_) --> getAll
		auto request = std::make_shared<GetWhilePatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
		result = pkb->get(request);
	} else if (type == ArgsType::QUOTE_WILDCARD)
	{
		// eg. pattern w ("y", _) --> get by arg1
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetWhilePatternRequest>(PatternMode::ARG1, arg1Value, EMPTY_ARG);
		result = pkb->get(request);
	}

	return result;
}


/**
* Get table of synonym values that match the while pattern.
*
* @param pt While pattern clause to be evaluated
* @param pkb Storage to be accessed
* @return table of synonym values that match the while pattern
*/
Table PatternWhileEvaluator::evaluateHasCommSyn(shared_ptr<PatternClause> pt, GetFacade* pkb)
{
	Argument arg1 = pt->arg_1;
	Argument arg2 = pt->arg_2;
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	PatternResults result = getAllMatches(pt, pkb);
	Table table;

	// Note: even if result is empty, still create a table with appropriate header to find intersection
	if (type == ArgsType::SYN_WILDCARD)
	{
		// eg. pattern w (v, _)
		Declaration arg1Dec = std::get<Declaration>(arg1.argumentVariant);
		table = QueryUtil::createTableArg1Arg2(pt->synonym.synName, arg1Dec.synName, result);
	}
	else {
		// eg. pattern w ("v", _), pattern w (_,_)
		table = QueryUtil::createTableArg1(pt->synonym.synName, result);
	}
	return table;

}