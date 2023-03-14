#include "FollowsTEvaluator.h"

FollowsTEvaluator::FollowsTEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

/**
* Evaluate clauses that have no synonyms.
*
* @param rel Follows* design abstraction to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for the design abstraction
*/
bool FollowsTEvaluator::evaluateClauseWithNoSyn()
{
	RelationshipResults result = getRelationPairs();

	return !result.empty();

}

/**
* Get all relation pairs that satisfy the Follows* relationship.
*
* @param rel Follows* relationship to be evaluated
* @param facade Storage to be accessed
* @return pairs of entities that satisfy Follows* relationship
*/
RelationshipResults FollowsTEvaluator::getRelationPairs()
{
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	RelationshipResults result;

	if (type == ArgsType::SYN_SYN) {
		if (QueryUtil::isSameSynonym(arg1, arg2)) {
			return result;
		}
	}

	if (type == ArgsType::STMTNO_STMTNO)
	{
		// eg. Follows*(1, 2)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetFollowsRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2, STAR);
		result = facade.get(request);
	}
	if (type == ArgsType::STMTNO_WILDCARD || type == ArgsType::STMTNO_SYN)
	{
		// eg. Follows*(1, _), Follows*(1, s)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetFollowsRequest>(arg1Value, Mode::FILTER_ARG_1, STAR);
		result = facade.get(request);
	}
	if (type == ArgsType::SYN_STMTNO || type == ArgsType::WILDCARD_STMTNO)
	{
		// eg. Follows*(s, 1), Follows*(_, 1)
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetFollowsRequest>(arg2Value, Mode::FILTER_ARG_2, STAR);
		result = facade.get(request);
	}
	if (type == ArgsType::SYN_SYN || type == ArgsType::SYN_WILDCARD ||
		type == ArgsType::WILDCARD_SYN || type == ArgsType::WILDCARD_WILDCARD)
	{
		// eg. Follows*(s1, s2), Follows*(s1, _), Follows*(_, s1), Follows*(_,_)
		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL, STAR);
		result = facade.get(request);
	}

	return result;
}
