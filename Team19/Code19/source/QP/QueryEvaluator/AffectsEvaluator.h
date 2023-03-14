#pragma once

#include <deque>
#include <set>
#include <memory>

#include "../../CFG/Cfg.h"
#include "../../PKB/GetFacade.h"
#include "../../TypeAlias/TypeAlias.h"
#include "../QueryObject/Argument.h"
#include "../QueryObject/DesignAbstraction.h"
#include "AffectsAlgorithm/AffectsStmtStmt.h"
#include "AffectsAlgorithm/AffectsStmtSyn.h"
#include "AffectsAlgorithm/AffectsStmtWild.h"
#include "AffectsAlgorithm/AffectsSynStmt.h"
#include "AffectsAlgorithm/AffectsSynSyn.h"
#include "AffectsAlgorithm/AffectsWildStmt.h"
#include "AffectsAlgorithm/AffectsWildWild.h"
#include "AffectsUtil.h"
#include "QueryUtil.h"
#include "RelationshipEvaluator.h"

using namespace std;

class AffectsEvaluator : public RelationshipEvaluator {
public:
    AffectsEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade);

    bool evaluateClauseWithNoSyn() override;
    RelationshipResults getRelationPairs() override;

private:

    /*
        Checks for a specific Affects relationship i.e. Affects(int,int)
    */
    bool checkSpecificAffects(const stmtNo_t& start, const stmtNo_t& end);

    /*
        Checks if target statement affects any other statements i.e. Affects(int,_)
    */
    bool checkIfAffects(stmtNo_t target);

    /*
        Checks if target statement is affected by any other statements i.e. Affects(_,int)
    */
    bool checkIfAffected(stmtNo_t target);

    /*
        Checks if any Affects relationship exists i.e. Affects(_,_)
    */
    bool checkIfAffectsExists();

    /*
        Gets all statements that are affected by the target statement i.e. Affects(int,syn)
    */
    RelationshipResults getAffectedStatements(stmtNo_t target);

    /*
        Gets all statements that affects the target statement i.e. Affects(syn,int)
    */
    RelationshipResults getAffectingStatements(stmtNo_t target);

    /*
        Gets all Affects relationships i.e. Affects(syn,syn), Affects(syn,_), Affects(_,syn)
    */
    RelationshipResults getAllAffects();

    /*
        Checks if the synonym is a valid type for affects
    */
    bool isValidSynType(Argument arg);
};
