#include "WhilePatternExtractor.h"

void WhilePatternExtractor::extractStatementListPattern(const vector<StatementNode>& statements) {
    for (const auto& statement : statements) {
        visit([this](auto& stmt) {
            extractStatementPattern(stmt);
            }, statement);
    }
}

void WhilePatternExtractor::extractStatementPattern(const unique_ptr<WhileStatementNode>& whiles) {
    vector<Ent::Variable> controlVariables;
    DesignExtractorUtil::extractConditionVariables(controlVariables, whiles->getCondition());

    Ptn::WhilePtn whilePattern{ whiles->getStatementNo(), controlVariables };
    whilePatterns.push_back(whilePattern);

    extractStatementListPattern(whiles->getStatements());
}

void WhilePatternExtractor::extractStatementPattern(const unique_ptr<IfStatementNode>& ifs) {
    extractStatementListPattern(ifs->getThenStatements());
    extractStatementListPattern(ifs->getElseStatements());
}

template<typename T>
void WhilePatternExtractor::extractStatementPattern(const T& stmt) {
    return;
}

WhilePatternExtractor::WhilePatternExtractor() {}

vector<Ptn::WhilePtn> WhilePatternExtractor::extractWhilePattern(const unique_ptr<ProgramNode>& AST) {
    for (const auto& procedure : AST->getProcedures()) {
        extractStatementListPattern(procedure->getStatements());
    }

    return whilePatterns;
}