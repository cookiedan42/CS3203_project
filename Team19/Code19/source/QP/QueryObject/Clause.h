#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <memory>
#include "../../PKB/GetFacade.h"
#include "../QueryEvaluator/QueryUtil.h"

using namespace std;

class Clause {
public:
	std::string clauseType;
	int penalty;

	Clause() = default;

	bool operator==(const Clause& clause) const;
	bool operator!=(const Clause& clause) const;

	bool operator<(const Clause& clause) const;

	virtual bool hasSyn();
	virtual vector<Declaration> getSynonyms();
	virtual std::unordered_set<std::string> getSynNames();
	virtual bool evaluateClauseWithNoSyn(GetFacade* pkb);
	virtual bool evaluateClauseWithSynToBool(GetFacade* pkb);
	virtual Table evaluateClauseWithSyn(GetFacade* pkb);
	virtual std::string printClause();
};


