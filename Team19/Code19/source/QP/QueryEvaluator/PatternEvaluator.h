#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_set>

#include "QueryUtil.h"
#include "../QueryObject/PatternClause.h"
#include "../../PKB/GetFacade.h"
#include "PatternAssignEvaluator.h"
#include "PatternWhileEvaluator.h"
#include "PatternIfEvaluator.h"

using namespace std;

class PatternEvaluator {

public:
    static bool evaluateNoCommSyn(shared_ptr<PatternClause> pt, GetFacade* facade);
    static Table evaluateHasCommSyn(shared_ptr<PatternClause> pt, GetFacade* facade);

private:
    // Disallow creating an instance of this object
    PatternEvaluator() {}
};

