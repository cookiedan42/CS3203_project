#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stdexcept>
#include "../../Relationship/Relationship.h"
#include "../AST/AST.h"
#include"../../CFG/Cfg.h"

using terminals_t = std::vector<stmtNo_t>;

class CfgExtractor {
private:
	std::vector<Cfg> cfgCollection = {};
public:
	std::vector<Cfg> extractCfg(const unique_ptr<SP::ProgramNode>& AST);
private:
	Cfg extractProc(const unique_ptr<SP::ProcedureNode>& procedure);
	terminals_t extractStatementList(Cfg& cfg, terminals_t prev, const std::vector<SP::StatementNode>& statements);
private:
	//Each statement should return the list of possible terminals

	template <typename T>
	terminals_t getTerminals(Cfg& cfg, terminals_t prev, const T& stmt);
	terminals_t getTerminals(Cfg& cfg, terminals_t prev, const unique_ptr<SP::WhileStatementNode>& stmt);
	terminals_t getTerminals(Cfg& cfg, terminals_t prev, const unique_ptr<SP::IfStatementNode>& stmt);

	std::string getStmtType(const unique_ptr<SP::ReadStatementNode>& stmt);
	std::string getStmtType(const unique_ptr<SP::PrintStatementNode>& stmt);
	std::string getStmtType(const unique_ptr<SP::AssignStatementNode>& stmt);
	std::string getStmtType(const unique_ptr<SP::WhileStatementNode>& stmt);
	std::string getStmtType(const unique_ptr<SP::IfStatementNode>& stmt);
	std::string getStmtType(const unique_ptr<SP::CallStatementNode>& stmt);
};
