#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <memory>

#include "../QueryObject/WithClause.h"
#include "../../PKB/GetFacade.h"
#include "QueryUtil.h"


using namespace std;

class WithEvaluator {

public:
    static bool evaluateNoCommSyn(shared_ptr<WithClause> withCl, GetFacade* facade);
    static Table evaluateHasCommSyn(shared_ptr<WithClause> withCl, GetFacade* facade);

private:
    // Disallow creating an instance of this object
    WithEvaluator() {}

    static Table evaluateWithClause(shared_ptr<WithClause> withCl, GetFacade* facade);
    static Table evaluateWith_SynRefInt_SynRefInt(shared_ptr<WithClause> withCl, GetFacade* facade);
    static Table evaluateWith_SynRefInt_Int(shared_ptr<WithClause> withCl, GetFacade* facade);
    static Table evaluateWith_Integer_SynRefInt(shared_ptr<WithClause> withCl, GetFacade* facade);
    static Table evaluateWith_SynRefName_SynRefName(shared_ptr<WithClause> withCl, GetFacade* facade);
    static Table evaluateWith_SynRefName_Quote(shared_ptr<WithClause> withCl, GetFacade* facade);
    static Table evaluateWith_Quote_SynRefName(shared_ptr<WithClause> withCl, GetFacade* facade);
};

