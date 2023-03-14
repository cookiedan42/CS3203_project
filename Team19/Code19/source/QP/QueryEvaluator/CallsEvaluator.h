#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "../../Request/GetRequests/GetRelationshipRequests/GetCallsRequest.h"
#include "RelationshipEvaluator.h"
#include "../QueryObject/SuchThatClause.h"
#include "./QueryUtil.h"

using namespace std;

class CallsEvaluator : public RelationshipEvaluator {
public:
    CallsEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade);

    bool evaluateClauseWithNoSyn() override;
    RelationshipResults getRelationPairs() override;
};
