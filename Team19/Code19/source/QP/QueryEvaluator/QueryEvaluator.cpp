#include "QueryEvaluator.h"

// Evaluate query object.
list<string> QueryEvaluator::evaluate(QueryObject queryObj, GetFacade* facade)
{

    list<string> consolidatedResult;
    SelectObject selectObj = queryObj.selectObj;

    // 1. Evaluate clauses with no synonym
    bool continueEval = evaluateClausesWithNoSynonym(queryObj, facade);
    if (continueEval) {
        // 2. Evaluate clauses not related to select clause
        continueEval = evaluateClausesNotRelatedToSelect(queryObj, facade);
    }

    // 2b. Evaluate SelectBoolean, since all clauses are not related to SelectBoolean
    if (selectObj.type == SelectType::BOOLEAN) {
        consolidatedResult = ResultConsolidator::selectBoolean(continueEval);
        return consolidatedResult;
    }

    if (continueEval) {

        Table consolidatedResultTable;
        // 3. Evaluate clauses related to select clause
        if (queryObj.clauseGroupsRelatedToSelect.size() != 0) {
            consolidatedResultTable = evaluateClausesRelatedToSelect(queryObj, facade);
            int numOfResults = consolidatedResultTable.getTableRows().size();
            if (numOfResults != 0) {
                // 4. Join declarations related to SelectClause to table
                consolidatedResultTable = evaluateSelectDeclarations(selectObj, facade, consolidatedResultTable);
            }
        }
        else {
            consolidatedResultTable = evaluateSelectDeclarations(selectObj, facade, consolidatedResultTable);
        }

        // 5. Consolidate results from table and select clause
        consolidatedResult = ResultConsolidator::selectTuple(selectObj, consolidatedResultTable);
    }

    return consolidatedResult;
}


// Evaluates results for clauses with no synonym to produce boolean result if results exist
bool QueryEvaluator::evaluateClausesWithNoSynonym(QueryObject queryObj, GetFacade* facade)
{
    bool continueEval = true;

    auto clausesWithNoSyn = queryObj.clausesWithNoSynonym;
    
    for (auto& clause : clausesWithNoSyn.getRelatedClauses()) {
        // Evaluate each clause with no syn, early breaking when result is false
        bool result = clause->evaluateClauseWithNoSyn(facade);
        if (!result) {
            continueEval = false;
            break;
        }
    }

    return continueEval;
}


// Evaluates results for clauses not related to Select clause to produce boolean result if results exist
bool QueryEvaluator::evaluateClausesNotRelatedToSelect(QueryObject queryObj, GetFacade* facade)
{
    bool continueEval = true;

    auto clauseGroupsNotRelatedToSelect = queryObj.clauseGroupsNotRelatedToSelect;

    for (auto& clauseGroup : clauseGroupsNotRelatedToSelect) {
        auto clausesInGroup = clauseGroup.getRelatedClauses();
        if (clausesInGroup.size() == 1) {
            // If its the only clause in group, table joining not required, directly evaluate to bool
            auto clause = clausesInGroup[0];
            bool result = clause->evaluateClauseWithSynToBool(facade);
            if (!result) {
                // Early termination
                continueEval = false;
                break;
            }
        }
        else {
            Table groupResultTable;
            for (auto& clause : clausesInGroup) {
                // Evaluate each clause in group and do table joining
                Table table = clause->evaluateClauseWithSyn(facade);
                int numOfResults = table.getTableRows().size();
                if (numOfResults == 0) {
                    // Early termination using empty table
                    groupResultTable = Table();
                    break;
                }
                else {
                    if (groupResultTable.tableSize() == 0) {
                        groupResultTable = table;
                    }
                    else {
                        groupResultTable = QueryUtil::joinTable(table, groupResultTable);
                    }
                }
            }
            int numOfTableRows = groupResultTable.getTableRows().size();
            if (numOfTableRows == 0) {
                // Early termination
                continueEval = false;
                break;
            }
        }
    }

    return continueEval;
}


// Evaluate clauses related to Select to produce result Table for joining and selecting
Table QueryEvaluator::evaluateClausesRelatedToSelect(QueryObject queryObj, GetFacade* facade)
{
    std::vector<ClauseGroup> clauseGroupsRelatedToSelect = queryObj.clauseGroupsRelatedToSelect;
    Table consolidatedResultTable;

    for (auto& clauseGroup : clauseGroupsRelatedToSelect) {

        // Evaluate each clause group
        auto clausesInGroup = clauseGroup.getRelatedClauses();
        Table groupResultTable;

        for (auto& clause : clausesInGroup) {
            // Evaluate each clause in group
            Table table = clause->evaluateClauseWithSyn(facade);
            int numOfResults = table.getTableRows().size();
            if (numOfResults == 0) {
                // Early termination using empty table for group result
                groupResultTable = Table();
                break;
            }
            else {
                // Clause has result, join to group result table
                if (groupResultTable.tableSize() == 0) {
                    groupResultTable = table;
                }
                else {
                    groupResultTable = QueryUtil::joinTable(table, groupResultTable);
                }
            }
        }
        int numOfGroupTableRows = groupResultTable.getTableRows().size();
        if (numOfGroupTableRows == 0) {
            // Early termination using empty table because group has empty result
            consolidatedResultTable = Table();
            break;
        }
        else {
            // Group has result, join to consolidated result table
            if (consolidatedResultTable.tableSize() == 0) {
                consolidatedResultTable = groupResultTable;
            }
            else {
                consolidatedResultTable = QueryUtil::joinTable(groupResultTable, consolidatedResultTable);
            }
        }
    }

    return consolidatedResultTable;
}

// Evaluates possible results for select declarations and combines the results into the provided table
Table QueryEvaluator::evaluateSelectDeclarations(SelectObject selectObj, GetFacade* facade, Table table)
{
    Table consolidatedResultTable = table;
    std::vector<std::string> headers = table.getTableHeader();
    for (auto& selectDecTypePair : selectObj.selectPairs) {
        Declaration selectDec = selectDecTypePair.first;
        ArgType selectDecType = selectDecTypePair.second;

        auto it = find(headers.begin(), headers.end(), selectDec.synName);
        // select declaration name not in headers, need add to table
        if (it == headers.end()) {
            consolidatedResultTable = addSynonymOrSynonymAttributeToTable(selectDec, selectDecType, facade, consolidatedResultTable);
        }
        // select declaration name in headers, check if its REF_INT/REF_NAME, join to existing table to filter results
        else {
            consolidatedResultTable = joinSynonymAttributeToTable(selectDec, selectDecType, facade, consolidatedResultTable);
        }
    }
    return consolidatedResultTable;
}

Table QueryEvaluator::addSynonymOrSynonymAttributeToTable(Declaration dec, ArgType decType, GetFacade* facade, Table table) {
    Table consolidatedResultTable = table;

    Table decTable;
    // if synonym
    if (decType == ArgType::SYN) {
        unordered_set<string> selectDecValues = dec.synType->getSynonymFromPKB(facade);
        decTable = QueryUtil::createTable(dec.synName, selectDecValues);
    }
    if (decType == ArgType::ATTR_REF_INT) {
        unordered_set<string> selectDecValues = dec.synType->getSynonymFromPKB(facade);
        decTable = QueryUtil::createTableOnEqualValues(dec.synName, dec.synName + REF_INT_STR, selectDecValues, selectDecValues);
    }
    if (decType == ArgType::ATTR_REF_NAME) {
        vector<vector<string>> decAttrItems = dec.synType->getSynonymWithAttrRefFromPKB(facade, dec.synName, dec.synName + REF_INT_NAME);
        decTable = Table({ dec.synName, dec.synName + REF_INT_NAME }, decAttrItems);
    }
    if (consolidatedResultTable.tableSize() == 0) {
        consolidatedResultTable = decTable;
    }
    else {
        consolidatedResultTable = QueryUtil::joinTable(consolidatedResultTable, decTable);
    }
    return consolidatedResultTable;
}

Table QueryEvaluator::joinSynonymAttributeToTable(Declaration dec, ArgType decType, GetFacade* facade, Table table) {
    Table consolidatedResultTable = table;
    Table decAttrTable;
    if (decType == ArgType::ATTR_REF_INT) {
        unordered_set<string> selectDecValues = dec.synType->getSynonymFromPKB(facade);
        decAttrTable = QueryUtil::createTableOnEqualValues(dec.synName, dec.synName + REF_INT_STR, selectDecValues, selectDecValues);
    }
    if (decType == ArgType::ATTR_REF_NAME) {
        vector<vector<string>> decAttrItems = dec.synType->getSynonymWithAttrRefFromPKB(facade, dec.synName, dec.synName + REF_INT_NAME);
        decAttrTable = Table({ dec.synName, dec.synName + REF_INT_NAME }, decAttrItems);
    }

    int numResults = decAttrTable.getTableRows().size();
    if (numResults != 0) {
        consolidatedResultTable = QueryUtil::joinTable(consolidatedResultTable, decAttrTable);
    }
    return consolidatedResultTable;
}