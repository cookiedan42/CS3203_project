#include "AffectsTStmtSyn.h"

AffectsTStmtSyn::AffectsTStmtSyn(GetFacade& facade, const Cfg& cfg) : facade(facade), cfg(cfg) {}

RelationshipResults AffectsTStmtSyn::evaluate(const stmtNo_t& start) {
	std::string modifiedVar = AffectsUtil::getVariableModified(facade, start);
	std::set<std::string> vars{ modifiedVar };

	stmtNo_t start2 = *(cfg.getNextof(start).begin());
	evaluate(start2, PROC_END, vars);

	std::string start_str = std::to_string(start);
	RelationshipResults out{};
	for (auto& syn : result) {
		out.insert({ start_str,std::to_string(syn) });
	}
	return out;
}

void AffectsTStmtSyn::evaluate(const stmtNo_t& start, const stmtNo_t& end, std::set<std::string>& vars) {
	auto current = start;
	while (current != end) {
		if (AffectsUtil::isIfStatement(cfg, current)) {
			handleIfStatement(current, vars);
			current = -current;
			continue;
		}

		if (AffectsUtil::isWhileStatement(cfg, current)) {
			// loop until stable
			handleWhileStatement(current, vars);

			// while has 2 nexts, w+1 and other
			auto nexts = cfg.getNextof(current);
			nexts.erase(current + 1);
			current = *(nexts.cbegin());
			continue;
		}

		if (AffectsUtil::isAssignStatement(cfg, current)) {
			handleAssignStatement(current, vars);
			current = *(cfg.getNextof(current).begin());
			continue;
		}

		auto modifiedVars = AffectsUtil::getAllVariablesModified(facade, current);
		for (const auto& modifiedVar : modifiedVars) vars.erase(modifiedVar);

		current = *(cfg.getNextof(current).begin());
	}
}
void AffectsTStmtSyn::handleIfStatement(const stmtNo_t& current, std::set<std::string>& vars){
	//fork join
	std::set<std::string> then_copy = vars;
	std::set<std::string> else_copy = vars;

	auto nexts = cfg.getNextof(current);
	stmtNo_t then_start = *(nexts.begin());
	stmtNo_t else_start = *(nexts.rbegin());

	evaluate(then_start, -current, then_copy);
	evaluate(else_start, -current, else_copy);

	then_copy.insert(else_copy.cbegin(), else_copy.cend());
	vars = then_copy;
}

void AffectsTStmtSyn::handleWhileStatement(const stmtNo_t& current, std::set<std::string>& vars) {
	std::set<std::string> old_vars{};
	std::set<std::string> child_vars{};
	while (vars != old_vars) {
		old_vars = vars;
		child_vars = vars;
		evaluate(current + 1, -current, child_vars);
		vars.insert(child_vars.cbegin(), child_vars.cend());
	}
}

void AffectsTStmtSyn::handleAssignStatement(const stmtNo_t& current, std::set<std::string>& vars) {

	auto usedVars = AffectsUtil::getVariablesUsed(facade, current);
	auto modifiedVar = AffectsUtil::getVariableModified(facade, current);

	for (auto& usedVar : usedVars) {
		if (vars.find(usedVar) == vars.cend()) { continue; }
		result.insert(current);
		vars.insert(modifiedVar);
		return;
	}
	// this stmt used no vars in vars
	vars.erase(modifiedVar);
}
