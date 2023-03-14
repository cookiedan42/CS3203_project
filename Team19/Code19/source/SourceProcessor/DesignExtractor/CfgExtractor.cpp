#include "CfgExtractor.h"

std::vector<Cfg> CfgExtractor::extractCfg(const unique_ptr<SP::ProgramNode>& AST) {
	for (const auto& procedure : AST->getProcedures()) {
		auto entry = extractProc(procedure);
		cfgCollection.push_back(entry);
	}
	return cfgCollection;
}

Cfg CfgExtractor::extractProc(const unique_ptr<SP::ProcedureNode>& procedure) {
	// all procedures must have at least 1 statement
	// enforced by SIMPLE validation
	auto firstStmtNo = std::visit([](auto&& statement) ->
		stmtNo_t {return statement->getStatementNo(); }, procedure->getStatements()[0]);
	auto cfg = Cfg(procedure->getName(), firstStmtNo);

	extractStatementList(cfg, terminals_t {}, procedure->getStatements());

	
	if (cfg.getNextof(cfg.getLastStmtNo()).size() == 0) {
		//normal statement
		cfg.insert(cfg.getLastStmtNo(), 0);
		return cfg;
	}

	for (stmtNo_t i = cfg.getFirstStmtNo(); i <=cfg.getLastStmtNo(); i++) {
		if (cfg.getStmtType(i) == Ent::while_stmtType
			&& cfg.getNextof(i).size()==1
			) {
			cfg.insert(i, 0);
			return cfg;
		}
		if (cfg.getStmtType(i) == Ent::if_stmtType
			&& cfg.getNextof(-i).size() == 0
			) {
			cfg.insert(-i, 0);
			return cfg;
		}
	}
}

terminals_t CfgExtractor::extractStatementList(Cfg& cfg, terminals_t prev, const std::vector<SP::StatementNode>& statements) {
	// statement list should return terminals of last executed statement
	for (const auto& statement : statements) {
		auto currentStmtNo = std::visit([](auto&& statement) ->
			stmtNo_t {return statement->getStatementNo(); }, statement);
		auto currentStmtType = std::visit([this](auto&& statement) ->
			std::string {return getStmtType(statement); }, statement);

		for each (auto p in prev) {
			cfg.insert(p, currentStmtNo);
		}
		cfg.insert(currentStmtNo, currentStmtType);
		prev = std::visit([this, prev, &cfg](auto&& statement) ->
			terminals_t {return getTerminals(cfg, prev, statement); }, statement);		
	}
	return prev;
}

// terminals of read, print, assign, call are only itself
template <typename T>
terminals_t CfgExtractor::getTerminals(Cfg & cfg, terminals_t prev, const T & stmt) {
	auto current = stmt->getStatementNo();
	return { current };
}

terminals_t CfgExtractor::getTerminals(Cfg& cfg, terminals_t prev, const unique_ptr<SP::WhileStatementNode>& stmt) {
	// while statement is the only exit
	// terminals of nest lead back to while statement
	auto current = stmt->getStatementNo();
	auto childTerminals = extractStatementList(cfg, { current }, stmt->getStatements());
	for each (auto p in childTerminals) {
		cfg.insert(p, -current);
	}
	cfg.insert(-current,current);
	return { current };
}

terminals_t CfgExtractor::getTerminals(Cfg& cfg, terminals_t prev, const unique_ptr<SP::IfStatementNode>& stmt) {
	// terminals of if is the combination of its' two substatements
	// insert dummy node
	auto current = stmt->getStatementNo();
	auto thenTerminals = extractStatementList(cfg,{ current }, stmt->getThenStatements());
	auto elseTerminals = extractStatementList(cfg,{ current }, stmt->getElseStatements());
	
	for each (auto p in thenTerminals) {
		cfg.insert(p, -current);
	}
	for each (auto p in elseTerminals) {
		cfg.insert(p, -current);
	}
	return { -current };
	// terminals --> dummy node --> Follows
}

std::string CfgExtractor::getStmtType(const unique_ptr<SP::ReadStatementNode>& stmt) { return Ent::read_stmtType; }
std::string CfgExtractor::getStmtType(const unique_ptr<SP::PrintStatementNode>& stmt) { return Ent::print_stmtType; }
std::string CfgExtractor::getStmtType(const unique_ptr<SP::AssignStatementNode>& stmt) { return Ent::assign_stmtType; }
std::string CfgExtractor::getStmtType(const unique_ptr<SP::WhileStatementNode>& stmt) { return Ent::while_stmtType; }
std::string CfgExtractor::getStmtType(const unique_ptr<SP::IfStatementNode>& stmt) { return Ent::if_stmtType; }
std::string CfgExtractor::getStmtType(const unique_ptr<SP::CallStatementNode>& stmt) { return Ent::call_stmtType; }
