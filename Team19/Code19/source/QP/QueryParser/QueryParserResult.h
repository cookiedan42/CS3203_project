#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "../QueryObject/Declaration.h"
#include "../QueryObject/PatternClause.h"
#include "../QueryObject/SuchThatClause.h"
#include "../QueryObject/WithClause.h"
#include "../QueryObject/SelectObject.h"
#include "../QueryObject/ClauseGroup.h"

class QueryParserResult {
public:
    QueryParserResult(std::vector<Declaration> declarations, SelectObject selectObj,
        std::vector<SuchThatClause> noSynSTClauses, std::vector<SuchThatClause> hasSynSTClauses,
        std::vector<PatternClause> noSynPTClauses, std::vector<PatternClause> hasSynPTClauses,
        std::vector<WithClause> noSynWithClauses, std::vector<WithClause> hasSynWithClauses
    );

    QueryParserResult(
        std::vector<Declaration> declarations,SelectObject selectObj,ClauseGroup noSynClauses,std::deque<ClauseGroup> clausesWithAtLeastOneSynonym) :
        declarations(declarations), selectObj(selectObj), noSynClauses(noSynClauses), clausesWithAtLeastOneSynonym(clausesWithAtLeastOneSynonym) 
    {}

    std::vector<Declaration> getDeclarations() const;
    SelectObject getSelectObj() const;
    ClauseGroup getNoSynClauses() const;
    std::deque<ClauseGroup> getClausesWithAtLeastOneSynonym() const;
    bool operator==(const QueryParserResult& other) const;

private:
    std::vector<Declaration> declarations;
    SelectObject selectObj;

    ClauseGroup noSynClauses;
    std::deque<ClauseGroup> clausesWithAtLeastOneSynonym;

    void processSuchThatClauses(std::vector<SuchThatClause>& noSynclauses, std::vector<SuchThatClause>& hasSynclauses);
    void processPatternClauses(std::vector<PatternClause>& noSynclauses, std::vector<PatternClause>& hasSynclauses);
    void processWithClauses(std::vector<WithClause>& noSynclauses, std::vector<WithClause>& hasSynclauses);

};




