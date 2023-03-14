#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "../../Request/GetRequests/GetRelationshipRequests/GetNextRequest.h"
#include "RelationshipEvaluator.h"
#include "../QueryObject/SuchThatClause.h"
#include "./QueryUtil.h"

using namespace std;

class NextEvaluator : public RelationshipEvaluator {
public:
    NextEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade);

    bool evaluateClauseWithNoSyn() override;
    RelationshipResults getRelationPairs() override;
};
