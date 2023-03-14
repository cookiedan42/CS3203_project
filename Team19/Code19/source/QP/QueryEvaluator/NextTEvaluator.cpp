#include "NextTEvaluator.h"

NextTEvaluator::NextTEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

/**
* Evaluate clauses that have no synonyms.
*
* @param rel Next* design abstraction to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for the design abstraction
*/
bool NextTEvaluator::evaluateClauseWithNoSyn()
{
	RelationshipResults result = getRelationPairs();

	return !result.empty();

}

/**
* Get all relation pairs that satisfy the Next* relationship.
*
* @param rel Next* relationship to be evaluated
* @param facade Storage to be accessed
* @return pairs of entities that satisfy Next* relationship
*/
RelationshipResults NextTEvaluator::getRelationPairs()
{
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	RelationshipResults result;
	shared_ptr<GetNextRequest> request;
	ArgString arg1Value;
	ArgString arg2Value;


	// Next* which has wildcard can be evaluated using Next() containing same args
	switch (type)
	{
	case ArgsType::SYN_SYN:
		// eg. statement p, q; Next(p, q)
		request = std::make_shared<GetNextRequest>(Mode::ALL, STAR);
		break;

	case ArgsType::SYN_WILDCARD:
	case ArgsType::WILDCARD_SYN:
	case ArgsType::WILDCARD_WILDCARD:
		// Next* with wildcard is same as Next with same args
		// eg. statement p, q; Next(p, _), Next(_, q), Next(_, _)
		request = std::make_shared<GetNextRequest>(Mode::ALL);
		break;

	case ArgsType::SYN_STMTNO:
		// eg. Next(p, 1)
		arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		request = std::make_shared<GetNextRequest>(arg2Value, Mode::FILTER_ARG_2, STAR);
		break;

	case ArgsType::WILDCARD_STMTNO:
		// eg. Next(_, 1)
		arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		request = std::make_shared<GetNextRequest>(arg2Value, Mode::FILTER_ARG_2);
		break;

	case ArgsType::STMTNO_SYN:
		// eg. Next(1, q)
		arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		request = std::make_shared<GetNextRequest>(arg1Value, Mode::FILTER_ARG_1, STAR);
		break;

	case ArgsType::STMTNO_WILDCARD:
		// eg. Next(1, _)
		arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		request = std::make_shared<GetNextRequest>(arg1Value, Mode::FILTER_ARG_1);
		break;

	case ArgsType::STMTNO_STMTNO:
		arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		request = std::make_shared<GetNextRequest>(arg1Value, arg2Value, Mode::FILTER_ARG1_ARG2, STAR);
		break;

	default:
		break;
	}

	result = facade.get(request);

	if (type == ArgsType::SYN_SYN && QueryUtil::isSameSynonym(arg1, arg2)) {
		result = QueryUtil::filterEqualPairs(result);
	}
	
	return result;
}
