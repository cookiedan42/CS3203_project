#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Clause.h"
#include "Declaration.h"

using namespace std;

class PatternClause: public Clause {
    public:

        Declaration synonym;
        Argument arg_1;
        Argument arg_2;

        bool operator==(const PatternClause& other) const;

        PatternClause(Declaration syn, Argument arg_1, Argument arg_2);

        bool hasSyn() override;
        vector<Declaration> getSynonyms() override;
        std::unordered_set<std::string> getSynNames() override;
        bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
        bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
        Table evaluateClauseWithSyn(GetFacade* pkb) override;
        std::string printClause() override;

        bool isIfPattern() const;
        bool isWhilePattern() const;
        bool isAssignPattern() const;



private:
    vector<Declaration> synonyms;
    std::unordered_set<std::string> synNames;
};
