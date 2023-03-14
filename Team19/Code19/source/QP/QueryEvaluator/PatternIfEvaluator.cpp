#include "PatternIfEvaluator.h"

/**
* Evaluate clauses that have no common synonym with other clauses.
*
* @param pt Pattern clause to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for evaluated pattern clause
*/
bool PatternIfEvaluator::evaluateNoCommSyn(shared_ptr<PatternClause> pt, GetFacade* facade)
{
	PatternResults result = getAllMatches(pt, facade);

	return !result.empty();

}

/**
* Get all pairs of if stmt_no and variable used that match the pattern.
*
* @param pt If pattern clause to be evaluated
* @param facade Storage to be accessed
* @return pairs of if stmt_no and variable used that match the pattern.
*/
PatternResults PatternIfEvaluator::getAllMatches(shared_ptr<PatternClause> pt, GetFacade* facade)
{
	Argument arg1 = pt->arg_1;
	Argument arg2 = pt->arg_2;
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	PatternResults result;

	if (type == ArgsType::SYN_WILDCARD || type == ArgsType::WILDCARD_WILDCARD)
	{
		// eg. pattern if (v, _), pattern if (_,_) --> getAll
		auto request = std::make_shared<GetIfPatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
		result = facade->get(request);
	}
	else if (type == ArgsType::QUOTE_WILDCARD)
	{
		// eg. pattern if ("y", _) --> get by arg1
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetIfPatternRequest>(PatternMode::ARG1, arg1Value, EMPTY_ARG);
		result = facade->get(request);
	}

	return result;
}


/**
* Get table of synonym values that match the If pattern.
*
* @param pt If pattern clause to be evaluated
* @param facade Storage to be accessed
* @return table of synonym values that match the If pattern
*/
Table PatternIfEvaluator::evaluateHasCommSyn(shared_ptr<PatternClause> pt, GetFacade* facade)
{
	Argument arg1 = pt->arg_1;
	Argument arg2 = pt->arg_2;
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	PatternResults result = getAllMatches(pt, facade);
	Table table;

	// Note: even if result is empty, still create a table with appropriate header to find intersection
	if (type == ArgsType::SYN_WILDCARD)
	{
		// eg. pattern if (v, _)
		Declaration arg1Dec = std::get<Declaration>(arg1.argumentVariant);
		table = QueryUtil::createTableArg1Arg2(pt->synonym.synName, arg1Dec.synName, result);
	}
	else {
		// eg. pattern if ("v", _), pattern if (_,_)
		table = QueryUtil::createTableArg1(pt->synonym.synName, result);
	}
	return table;

}