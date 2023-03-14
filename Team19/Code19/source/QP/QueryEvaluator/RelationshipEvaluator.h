#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "../../PKB/GetFacade.h"
#include "../QueryObject/SuchThatClause.h"
#include "./QueryUtil.h"

using namespace std;

class RelationshipEvaluator {
public:
    RelationshipEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade);

    // methods common to all relationship evaluators
    virtual RelationshipResults getRelationPairs() = 0;
    virtual bool evaluateClauseWithNoSyn() = 0;

    bool evaluateClauseWithSynToBool();
    Table evaluateClauseWithSyn();

private:
    Table createTableOnArg1StatementType(Declaration dec, GetFacade& facade, RelationshipResults resultPairs);
    Table createTableOnArg2StatementType(Declaration dec, GetFacade& facade, RelationshipResults resultPairs);
    Table createTableOnArg1Arg2StatementType(Declaration dec1, Declaration dec2, GetFacade& facade, RelationshipResults resultPairs);

protected:
    Argument arg1;
    Argument arg2;
    GetFacade facade;

};
