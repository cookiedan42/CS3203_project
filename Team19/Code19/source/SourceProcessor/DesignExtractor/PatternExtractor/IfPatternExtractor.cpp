#include "IfPatternExtractor.h"

void IfPatternExtractor::extractStatementListPattern(const vector<StatementNode>& statements) {
    for (const auto& statement : statements) {
        visit([this](auto& stmt) {
            extractStatementPattern(stmt);
            }, statement);
    }
}

void IfPatternExtractor::extractStatementPattern(const unique_ptr<WhileStatementNode>& whiles) {
    extractStatementListPattern(whiles->getStatements());
}

void IfPatternExtractor::extractStatementPattern(const unique_ptr<IfStatementNode>& ifs) {
    vector<Ent::Variable> controlVariables;
    DesignExtractorUtil::extractConditionVariables(controlVariables, ifs->getCondition());

    Ptn::IfPtn ifPattern{ ifs->getStatementNo(), controlVariables };
    ifPatterns.push_back(ifPattern);

    extractStatementListPattern(ifs->getThenStatements());
    extractStatementListPattern(ifs->getElseStatements());
}

template<typename T>
void IfPatternExtractor::extractStatementPattern(const T& stmt) {
    return;
}

IfPatternExtractor::IfPatternExtractor() {}

vector<Ptn::IfPtn> IfPatternExtractor::extractIfPattern(const unique_ptr<ProgramNode>& AST) {
    for (const auto& procedure : AST->getProcedures()) {
        extractStatementListPattern(procedure->getStatements());
    }

    return ifPatterns;
}