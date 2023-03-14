#include "PatternEvaluator.h"
#include "../QueryObject/DesignEntity.h"

using namespace std;

bool PatternEvaluator::evaluateNoCommSyn(shared_ptr<PatternClause> pt, GetFacade* facade)
{
    bool result = false;
    std::string type = pt->synonym.synType->type;

    if (type == DE::Assign().type)
    {
        result = PatternAssignEvaluator::evaluateNoCommSyn(pt, facade);
    }
    if (type == DE::While().type)
    {
        result = PatternWhileEvaluator::evaluateNoCommSyn(pt, facade);
    }
    if (type == DE::If().type)
    {
        result = PatternIfEvaluator::evaluateNoCommSyn(pt, facade);
    }
   
    return result;
}

Table PatternEvaluator::evaluateHasCommSyn(shared_ptr<PatternClause> pt, GetFacade* facade)
{

    Table table;

    std::string type = pt->synonym.synType->type;
 
    if (type == DE::Assign().type)
    {
        table = PatternAssignEvaluator::evaluateHasCommSyn(pt, facade);
    }
    if (type == DE::While().type)
    {
        table = PatternWhileEvaluator::evaluateHasCommSyn(pt, facade);
    }
    if (type == DE::If().type)
    {
        table = PatternIfEvaluator::evaluateHasCommSyn(pt, facade);
    }
   
    return table;

}
