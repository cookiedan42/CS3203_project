#pragma once

#include <deque>
#include <map>
#include <set>
#include <memory>

#include "../../CFG/Cfg.h"
#include "../../PKB/GetFacade.h"
#include "../../TypeAlias/TypeAlias.h"
#include "../QueryObject/Argument.h"
#include "../QueryObject/DesignAbstraction.h"
#include "AffectsTAlgorithm/AffectsTStmtStmt.h"
#include "AffectsTAlgorithm/AffectsTStmtSyn.h"
#include "AffectsTAlgorithm/AffectsTSynStmt.h"
#include "AffectsTAlgorithm/AffectsTSynSyn.h"
#include "AffectsEvaluator.h"
#include "AffectsUtil.h"
#include "QueryUtil.h"
#include "RelationshipEvaluator.h"

using namespace std;

class AffectsTEvaluator : public RelationshipEvaluator {
public:
    AffectsTEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade);

    bool evaluateClauseWithNoSyn() override;
    RelationshipResults getRelationPairs() override;

private:
    /*
        Checks for a specific AffectsT relationship i.e. Affects*(int,int)
    */
    bool checkSpecificAffectsT(const stmtNo_t& start, const stmtNo_t& end);

    /*
        Gets all statements that affectsT the target statement i.e. Affects*(syn,int)
    */
    RelationshipResults getAffectingTStatements(stmtNo_t target);

    /*
        Gets all statements that are affectedT by the target statement i.e. Affects*(int,syn)
    */
    RelationshipResults getAffectedTStatements(stmtNo_t target);

    /*
        Gets all AffectsT relationships i.e. Affects*(syn,syn)
    */
    RelationshipResults getAllAffectsT();

    /*
    Checks if the synonym is a valid type for affects*
    */
    bool isValidSynType(Argument arg);

};
