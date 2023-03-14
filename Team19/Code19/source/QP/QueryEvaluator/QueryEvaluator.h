#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../QueryObject/QueryObject.h"
#include "QueryUtil.h"
#include "../../PKB/GetFacade.h"
#include "ResultConsolidator.h"
#include "SuchThatEvaluator.h"
#include "PatternEvaluator.h"
#include "WithEvaluator.h"

using namespace std;

class QueryEvaluator {

    public:
        static list<string> evaluate(QueryObject queryObj, GetFacade* facade);

    private:
        static bool evaluateClausesWithNoSynonym(QueryObject obj, GetFacade* facade);
        static bool evaluateClausesNotRelatedToSelect(QueryObject obj, GetFacade* facade);
        static Table evaluateClausesRelatedToSelect(QueryObject queryObj, GetFacade* facade);
        static Table evaluateSelectDeclarations(SelectObject selectObj, GetFacade* facade, Table table);
        static Table addSynonymOrSynonymAttributeToTable(Declaration dec, ArgType decType, GetFacade* facade, Table table);
        static Table joinSynonymAttributeToTable(Declaration dec, ArgType attrType, GetFacade* facade, Table table);

};
