#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "../../Request/GetRequests/GetPatternRequests/GetAssignPatternRequest.h"
#include "../QueryObject/PatternClause.h"
#include "QueryUtil.h"

using namespace std;

class PatternAssignEvaluator {

public:
    static bool evaluateNoCommSyn(shared_ptr<PatternClause> pt, GetFacade* pkb);
    static Table evaluateHasCommSyn(shared_ptr<PatternClause> pt, GetFacade* pkb);
    static PatternResults getAllMatches(shared_ptr<PatternClause> pt, GetFacade* pkb);

private:
    // Disallow creating an instance of this object
    PatternAssignEvaluator() {}

};
