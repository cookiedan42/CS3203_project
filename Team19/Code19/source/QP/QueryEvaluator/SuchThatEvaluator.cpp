#include "SuchThatEvaluator.h"
#include "../QueryObject/DesignEntity.h"

using namespace std;

/**
* Evaluate such that clauses that have no synonym.
*
* @param st Such-that clause to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for evaluated such-that clause
*/
bool SuchThatEvaluator::evaluateClauseWithNoSyn(shared_ptr<SuchThatClause> st, GetFacade* facade)
{
    return st->relationship->evaluateClauseWithNoSyn(facade);
}

/**
* Evaluate such that clauses that have no common synonym with other clauses.
*
* @param st Such-that clause to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for evaluated such-that clause
*/
bool SuchThatEvaluator::evaluateClauseWithSynToBool(shared_ptr<SuchThatClause> st, GetFacade* facade)
{
    return st->relationship->evaluateClauseWithSynToBool(facade);
}

/**
* Evaluate such that clauses that have common synonym with other clauses, creating a table for comparison.
*
* @param st Such-that clause to be evaluated
* @param facade Storage to be accessed
* @return table of synonym values that satisfy the such that clause
*/
Table SuchThatEvaluator::evaluateClauseWithSyn(shared_ptr<SuchThatClause> st, GetFacade* facade)
{
    return st->relationship->evaluateClauseWithSyn(facade);
}
