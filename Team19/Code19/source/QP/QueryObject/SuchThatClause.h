#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Clause.h"
#include "DesignAbstraction.h"

using namespace std;

class SuchThatClause : public Clause {
    public:
        shared_ptr<DesignAbstraction> relationship;
        
        bool operator==(const SuchThatClause& other) const;

        SuchThatClause(shared_ptr<DesignAbstraction> relationship);
        
        bool hasSyn() override;
        vector<Declaration> getSynonyms() override;
        std::unordered_set<std::string> getSynNames() override;
        bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
        bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
        Table evaluateClauseWithSyn(GetFacade* pkb) override;
        Argument::Arg getArg1() const { return relationship->arg1.argumentVariant; }
        Argument::Arg getArg2() const { return relationship->arg2.argumentVariant; }
        std::string printClause() override;
private:
    vector<Declaration> synonyms;
    std::unordered_set<std::string> synNames;
};
