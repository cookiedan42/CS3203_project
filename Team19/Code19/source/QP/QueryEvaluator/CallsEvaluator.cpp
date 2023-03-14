#include "CallsEvaluator.h"

CallsEvaluator::CallsEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

/**
* Evaluate clauses that have no synonyms.
*
* @param rel Calls design abstraction to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for the design abstraction
*/
bool CallsEvaluator::evaluateClauseWithNoSyn()
{
	RelationshipResults result = getRelationPairs();

	return !result.empty();

}

/**
* Get all relation pairs that satisfy the Calls design abstraction.
*
* @param rel Calls design abstraction to be evaluated
* @param facade Storage to be accessed
* @return pairs of entities that satisfy Calls design abstraction
*/
RelationshipResults CallsEvaluator::getRelationPairs()
{
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	RelationshipResults result;

	if (type == ArgsType::SYN_SYN) {
		if (QueryUtil::isSameSynonym(arg1, arg2)) {
			return result;
		}
	}

	if (type == ArgsType::SYN_SYN || type == ArgsType::SYN_WILDCARD 
		|| type == ArgsType::WILDCARD_SYN || type == ArgsType::WILDCARD_WILDCARD)
	{
		// eg. procedure p, q; Calls(p, q), Calls(p, _), Calls(_, q), Calls(_, _)
		auto request = std::make_shared<GetCallsRequest>(Mode::ALL);
		result = facade.get(request);
	}
	if (type == ArgsType::QUOTE_SYN || type == ArgsType::QUOTE_WILDCARD)
	{
		// eg. Calls("proc_name", q), Calls("proc_name", _)
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetCallsRequest>(arg1Value, Mode::FILTER_ARG_1);
		result = facade.get(request);
	}
	if (type == ArgsType::SYN_QUOTE || type == ArgsType::WILDCARD_QUOTE)
	{
		// eg. Calls(p, "proc_name"), Calls(_, "proc_name")
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetCallsRequest>(arg2Value, Mode::FILTER_ARG_2);
		result = facade.get(request);
	}
	if (type == ArgsType::QUOTE_QUOTE)
	{
		// eg. Calls("proc_name1", "proc_name2")
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetCallsRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2);
		result = facade.get(request);
	}

	return result;
}
