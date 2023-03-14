#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_set>

#include "../QueryObject/SuchThatClause.h"
#include "../../PKB/GetFacade.h"
#include "QueryUtil.h"
#include "FollowsEvaluator.h"
#include "FollowsTEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentTEvaluator.h"
#include "UsesEvaluator.h"
#include "ModifiesEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsTEvaluator.h"
#include "NextEvaluator.h"
#include "NextTEvaluator.h"


using namespace std;

class SuchThatEvaluator {

public:
    static bool evaluateClauseWithNoSyn(shared_ptr<SuchThatClause> st, GetFacade* facade);
    static bool evaluateClauseWithSynToBool(shared_ptr<SuchThatClause> st, GetFacade* facade);
    static Table evaluateClauseWithSyn(shared_ptr<SuchThatClause> st, GetFacade* facade);

private:
    // Disallow creating an instance of this object
    SuchThatEvaluator() {}
};

