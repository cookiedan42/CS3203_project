#include "AssignPatternExtractor.h"

void AssignPatternExtractor::extractStatementListPattern(const vector<StatementNode>& statements) {
    for (const auto& statement : statements) {
        visit([this](auto& stmt) {
            extractStatementPattern(stmt);
            }, statement);
    }
}

void AssignPatternExtractor::extractStatementPattern(const unique_ptr<AssignStatementNode>& assign) {
    string preOrderRHS = DesignExtractorUtil::getPreOrderTraversal(assign->getRHS());
    string inOrderRHS = DesignExtractorUtil::getInOrderTraversal(assign->getRHS());

    Ptn::AssignPtn pattern{ assign->getStatementNo(), assign->getLHS()->getName(), preOrderRHS, inOrderRHS };
    assignPatterns.push_back(pattern);
}

void AssignPatternExtractor::extractStatementPattern(const unique_ptr<WhileStatementNode>& whiles) {
    extractStatementListPattern(whiles->getStatements());
}

void AssignPatternExtractor::extractStatementPattern(const unique_ptr<IfStatementNode>& ifs) {
    extractStatementListPattern(ifs->getThenStatements());
    extractStatementListPattern(ifs->getElseStatements());
}

template<typename T>
void AssignPatternExtractor::extractStatementPattern(const T& stmt) {
    return;
}

AssignPatternExtractor::AssignPatternExtractor() {}

vector<Ptn::AssignPtn> AssignPatternExtractor::extractAssignPattern(const unique_ptr<ProgramNode>& AST) {
    for (const auto& procedure : AST->getProcedures()) {
        extractStatementListPattern(procedure->getStatements());
    }

    return assignPatterns;
}