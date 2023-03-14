#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../QueryEvaluator/QueryUtil.h"
#include "Declaration.h"
#include "Clause.h"

using namespace std;

/*
* Type of argument in With Clause.
* Note: 
* - syn.stmtno, syn.value is attrRef classified as ATTR_REF_INT
* - syn.varName, syn.value, syn.procName is classified as ATTR_REF_NAME
*/ 


class WithClause : public Clause
{
public:
    // fields
    Declaration dec1;
    Declaration dec2;
    std::string value1;
    std::string value2;
    ArgsType argsType;

    Argument arg1 = Argument{ std::monostate{} };
    Argument arg2 = Argument{ std::monostate{} };

    // constructors
    WithClause();
    WithClause(Argument arg1, Argument arg2);
    // methods
    bool operator==(const WithClause& other) const;

    bool hasSyn() override;
    vector<Declaration> getSynonyms() override;
    std::unordered_set<std::string> getSynNames() override;
    bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
    bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
    Table evaluateClauseWithSyn(GetFacade* pkb) override;
    std::string printClause() override;

private:
    shared_ptr<WithClause> makeSharedPtrWithClause(Declaration dec1, Declaration dec2,
        std::string value1, std::string value2, ArgsType argsType);
    vector<Declaration> synonyms;
    std::unordered_set<std::string> synNames;   
};
