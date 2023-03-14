#include "AffectsTStmtStmt.h"

AffectsTStmtStmt::AffectsTStmtStmt(GetFacade& facade, const Cfg& cfg, stmtNo_t target) :
	facade(facade), cfg(cfg), target(target) {}

bool AffectsTStmtStmt::evaluate(const stmtNo_t& start, const stmtNo_t& end) {
	std::string modifiedVar = AffectsUtil::getVariableModified(facade, start);
	std::set<std::string> vars { modifiedVar };

	stmtNo_t start2 = *(cfg.getNextof(start).begin());
	evaluate(start2, PROC_END, vars);
	return result;
}


void AffectsTStmtStmt::evaluate(const stmtNo_t & start, const stmtNo_t & end, std::set<std::string>&vars) {
	auto current = start;

	while (current != end) {
		if (AffectsUtil::isIfStatement(cfg, current)) {
			handleIfStatement(current, vars);
			if (result) return;

			current = -current;
			if (!cfg.hasPath_node(current, target)) { return; }
			continue;
		}

		if (AffectsUtil::isWhileStatement(cfg, current)) {
			handleWhileStatement(current, vars);

			if (result) return;

			// while has 2 nexts, w+1 and other
			auto nexts = cfg.getNextof(current);
			nexts.erase(current + 1);
			current = *(nexts.cbegin());

			if (current != target && !cfg.hasPath_node(current, target)) { return; }
			continue;
		}

		if (AffectsUtil::isAssignStatement(cfg, current)) {
			handleAssignStatement(current, vars);
			if (!cfg.hasPath_node(current, target)) return;
			current = *(cfg.getNextof(current).begin());
			continue;
		}

		auto modifiedVars = AffectsUtil::getAllVariablesModified(facade, current);
		for (const auto& modifiedVar : modifiedVars) vars.erase(modifiedVar);

		current = *(cfg.getNextof(current).begin());
	}
}

void AffectsTStmtStmt::handleIfStatement(const stmtNo_t& current, std::set<std::string>& vars){
	//fork join
	std::set<std::string> then_copy = vars;
	std::set<std::string> else_copy = vars;

	auto nexts = cfg.getNextof(current);
	stmtNo_t then_start = *(nexts.begin());
	stmtNo_t else_start = *(nexts.rbegin());

	evaluate(then_start, -current, then_copy);
	if (result) return;
	evaluate(else_start, -current, else_copy);
	if (result) return;

	then_copy.insert(else_copy.cbegin(), else_copy.cend());
	vars = then_copy;
}

void AffectsTStmtStmt::handleWhileStatement(const stmtNo_t& current, std::set<std::string>& vars) {
	// loop until stable
	std::set<std::string> old_vars;
	std::set<std::string> child_vars;
	while (vars != old_vars) {
		old_vars = vars;
		child_vars = vars;
		evaluate(current + 1, -current, child_vars);
		if (result) return;
		vars.insert(child_vars.cbegin(), child_vars.cend());
	}
}

void AffectsTStmtStmt::handleAssignStatement(const stmtNo_t& current, std::set<std::string>& vars) {
	auto usedVars = AffectsUtil::getVariablesUsed(facade, current);
	std::string modifiedVar = AffectsUtil::getVariableModified(facade, current);

	bool used = false;
	for (auto& usedVar : usedVars) {
		if (!vars.count(usedVar)) continue;

		if (current == target) {
			result = true;
			return;
		}
		vars.insert(modifiedVar);
		used = true;
		break;
	}
	if (!used) vars.erase(modifiedVar);
}
