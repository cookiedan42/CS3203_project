#include "ParentExtractor.h"

ParentExtractor::ParentExtractor() {}

vector<Rel::Parent> ParentExtractor::extractParent(const unique_ptr<ProgramNode> &AST) {
    for (const auto& procedure : AST->getProcedures()) {
        extractProc(procedure);
    }
    return parentRelationships;
}

void ParentExtractor::extractProc(const unique_ptr<ProcedureNode>& procedure) {
    extractStatementList(dummyStatement, procedure->getStatements());
}

void ParentExtractor::extractStatementList(const Ent::Statement& parent, const vector<StatementNode>& statements) {
    // if statement list belongs to a parent statement, populate
    if (parent != dummyStatement) {
        for (const auto& statement : statements) {
            auto childStmt = std::visit([this](auto&& statement) -> Ent::Statement {
                return DesignExtractorUtil::extractStatementEntity(statement); }, statement);

            Rel::Parent p{ parent, childStmt };
            parentRelationships.push_back(move(p));
        }
    }

    // check children for nests
    for (const auto& statement : statements) {
        std::visit([this](auto& stmt) {extractSublist(stmt); }, statement);
    }
}

void ParentExtractor::extractSublist(const unique_ptr<WhileStatementNode>& stmt) {
        extractStatementList(Ent::WhileStatement(stmt->getStatementNo()), stmt->getStatements());
}
void ParentExtractor::extractSublist(const unique_ptr<IfStatementNode>& stmt) {
    extractStatementList(Ent::IfStatement(stmt->getStatementNo()), stmt->getElseStatements());
    extractStatementList(Ent::IfStatement(stmt->getStatementNo()), stmt->getThenStatements());
}

template<typename T>
void ParentExtractor::extractSublist(const T& stmt) { return; }

