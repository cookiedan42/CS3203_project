#include "ParentEvaluator.h"

ParentEvaluator::ParentEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

/**
* Evaluate clauses that have no synonyms.
*
* @param rel Parent design abstraction to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for the design abstraction
*/
bool ParentEvaluator::evaluateClauseWithNoSyn()
{
	RelationshipResults result = getRelationPairs();

	return !result.empty();

}

/**
* Get all relation pairs that satisfy the Parent relationship.
*
* @param rel Parent relationship to be evaluated
* @param facade Storage to be accessed
* @return pairs of entities that satisfy Parent relationship
*/
RelationshipResults ParentEvaluator::getRelationPairs()
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
		// eg. Parent(1, 2)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetParentRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2);
		result = facade.get(request);
	}
	if (type == ArgsType::STMTNO_WILDCARD || type == ArgsType::STMTNO_SYN)
	{
		// eg. Parent(1, _), Parent(1, s)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		auto request = std::make_shared<GetParentRequest>(arg1Value, Mode::FILTER_ARG_1);
		result = facade.get(request);
	}
	if (type == ArgsType::SYN_STMTNO || type == ArgsType::WILDCARD_STMTNO)
	{
		// eg. Parent(s, 1), Parent(_, 1)
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		auto request = std::make_shared<GetParentRequest>(arg2Value, Mode::FILTER_ARG_2);
		result = facade.get(request);
	}
	if (type == ArgsType::SYN_SYN || type == ArgsType::SYN_WILDCARD ||
		type == ArgsType::WILDCARD_SYN || type == ArgsType::WILDCARD_WILDCARD)
	{
		// eg. Parent(s1, s2), Parent(s1, _), Parent(_, s1), Parent(_,_)
		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		result = facade.get(request);
	}
	return result;
}
