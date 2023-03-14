#pragma once
#include <vector>
#include "../../../Relationship/Relationship.h"
#include "../../AST/AST.h"

class CallsExtractor {
private:
	// to make global
	using procName = std::string;
private:
	vector<Rel::Calls> callsRelationships{};

public:
	std::vector<Rel::Calls> extractCalls(const unique_ptr<SP::ProgramNode>& AST);
	CallsExtractor() {};

private:
	void extractProc(const unique_ptr<SP::ProcedureNode>& procedure);
	void extractStatementList(procName parentName, const std::vector<SP::StatementNode>& statements);

private:

	void extractSublist(procName parentName, const unique_ptr<SP::WhileStatementNode>& stmt);
	void extractSublist(procName parentName, const unique_ptr<SP::IfStatementNode>& stmt);
	template<typename T>
	void extractSublist(procName parentName, const T& stmt);

private:

	void getStmt(procName parentName, const unique_ptr<SP::CallStatementNode>& stmt);
	template<typename T>
	void getStmt(procName parentName, const T& stmt);
};
