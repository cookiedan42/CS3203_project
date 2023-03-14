#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Declaration.h"
#include "PatternClause.h"
#include "SuchThatClause.h"
#include "WithClause.h"
#include "SelectObject.h"
#include "./ClauseGroup.h"

using namespace std;

class QueryObject {
public:
    std::vector<Declaration> declarations;
    SelectObject selectObj;

    // New design of query object
    ClauseGroup clausesWithNoSynonym;
    std::vector<ClauseGroup> clauseGroupsRelatedToSelect;
    std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect;
    
    QueryObject(SelectObject selectObj, std::vector<Declaration> declarations = {},
        ClauseGroup clausesWithNoSynonym = {},
        std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {},
        std::vector<ClauseGroup> clauseGroupsRelatedToSelect = {}
    );

    //bool operator==(const QueryObject& qo) const;
};
