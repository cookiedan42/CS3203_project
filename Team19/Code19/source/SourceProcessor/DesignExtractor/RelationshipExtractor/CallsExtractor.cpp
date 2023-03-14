#include "CallsExtractor.h"

std::vector<Rel::Calls> CallsExtractor::extractCalls(const unique_ptr<SP::ProgramNode>& AST) {
    for (const auto& procedure : AST->getProcedures()) {
        extractProc(procedure);
    }
    return callsRelationships;
}

void CallsExtractor::extractProc(const unique_ptr<SP::ProcedureNode>& procedure) {
    extractStatementList(procedure->getName(), procedure->getStatements());
}

void CallsExtractor::extractStatementList(procName parentName, const std::vector<SP::StatementNode>& statements) {
    for (const auto& statement : statements) {
        std::visit([this, parentName ]( auto&& statement){getStmt(parentName,statement); }, statement);
        std::visit([this,parentName](auto& stmt) {extractSublist(parentName,stmt); }, statement);
    }
}

template<typename T>
void CallsExtractor::extractSublist(procName parentName, const T& stmt) { return; }

void CallsExtractor::extractSublist(procName parentName, const unique_ptr<SP::WhileStatementNode>& stmt) {
    extractStatementList(parentName, stmt->getStatements());
}

void CallsExtractor::extractSublist(procName parentName, const unique_ptr<SP::IfStatementNode>& stmt) {
    extractStatementList(parentName, stmt->getElseStatements());
    extractStatementList(parentName, stmt->getThenStatements());
}

template<typename T>
void CallsExtractor::getStmt(procName parentName, const T& stmt) { return; }

void CallsExtractor::getStmt(procName parentName, const unique_ptr<SP::CallStatementNode>& stmt) {
    Ent::Procedure caller{ parentName };
    Ent::Procedure callee{ stmt->getProcName() };
    Rel::Calls call{ caller, callee };
    callsRelationships.push_back(move(call));
}