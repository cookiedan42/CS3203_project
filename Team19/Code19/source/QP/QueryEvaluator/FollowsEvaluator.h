#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "../../Request/GetRequests/GetRelationshipRequests/GetFollowsRequest.h"
#include "RelationshipEvaluator.h"
#include "../QueryObject/SuchThatClause.h"
#include "./QueryUtil.h"

using namespace std;

class FollowsEvaluator : public RelationshipEvaluator {
public:
    FollowsEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade);

    bool evaluateClauseWithNoSyn() override;
    RelationshipResults getRelationPairs() override;
};
