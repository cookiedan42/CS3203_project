#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor() {}

vector<Rel::Follows> FollowsExtractor::extractFollows(const unique_ptr<ProgramNode> &AST) {
    for (const auto& procedure : AST->getProcedures()) {
        extractProc(procedure);
    }
    return followsRelationships;
}

void FollowsExtractor::extractProc(const unique_ptr<ProcedureNode>& procedure) {
    extractStatementList(procedure->getStatements());
}

void FollowsExtractor::extractStatementList(const vector<StatementNode>& statements) {
    std::vector<Ent::Statement> lines {};
    for (const auto& statement : statements) {
        auto childStmt = std::visit([this](auto&& statement) -> Ent::Statement {
            return DesignExtractorUtil::extractStatementEntity(statement); }, statement);
        lines.push_back(childStmt);
        
        // check child for nested statement lists
        std::visit([this](auto& stmt) {extractSublist(stmt);}, statement);
    }

    // follows only makes sense when there are >= 2 statements
    if (lines.size() <= 1) { return; }
    for ( auto it = lines.cbegin(); it+1 != lines.cend(); it++){
        Rel::Follows follows{ *it, *(it + 1) };
        followsRelationships.push_back(move(follows));
    }
}

void FollowsExtractor::extractSublist(const unique_ptr<WhileStatementNode>& stmt) {
    extractStatementList(stmt->getStatements());
}
void FollowsExtractor::extractSublist(const unique_ptr<IfStatementNode>& stmt) {
    extractStatementList(stmt->getElseStatements());
    extractStatementList(stmt->getThenStatements());
}
template<typename T>
void FollowsExtractor::extractSublist(const T& stmt) { return; }
