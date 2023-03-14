#include "AffectsTSynStmt.h"

AffectsTSynStmt::AffectsTSynStmt(GetFacade& facade, const Cfg& cfg) : facade(facade), cfg(cfg) {}

RelationshipResults AffectsTSynStmt::evaluate(const stmtNo_t& end) {
	// assume that end is end is an insert statement
	
	auto usedVars = AffectsUtil::getVariablesUsed(facade, end);
	std::set<std::string> vars{ usedVars.cbegin(),usedVars.cend() };
	auto end2 = *(cfg.getPrevof(end).begin());

	evaluate(cfg.getFirstStmtNo(), end2, vars);

	std::string end_str = std::to_string(end);
	RelationshipResults out{};
	for (auto& syn : result) {
		out.insert({ std::to_string(syn),end_str });
	}
	return out;
}




void AffectsTSynStmt::evaluate(
	const stmtNo_t& start, const stmtNo_t& end,
	std::set<std::string>& vars) {

	auto current = end;

	while (current != start) { // need to do once more after current == start
	
		if (AffectsUtil::isIfStatement(cfg, current)) {
			current = *cfg.getPrevof(current).begin();
			continue;
		}

		if (AffectsUtil::isIfStatement(cfg, -current)) {
			handleIfStatement(current, vars);
			current = -current;
			continue;
		}

		if (AffectsUtil::isWhileStatement(cfg, current)) {
			handleWhileStatement(current, vars);

			// prev is either -current or something else
			auto a = cfg.getPrevof(current);
			a.erase(-current);
			current = *(a.cbegin());
			continue;
		}

		if (AffectsUtil::isAssignStatement(cfg, current)) {
			handleAssignStatement(current, vars);
			current = *(cfg.getPrevof(current).begin());
			continue;
		}

		// read,call,print
		// always erase modified var from table
		auto modifiedVars = AffectsUtil::getAllVariablesModified(facade, current);
		for (auto& modifiedVar : modifiedVars) vars.erase(modifiedVar);
		// these statements always have one prev
		current = *(cfg.getPrevof(current).begin());
	}

	// one more cycle to resolve first statement
	// matters if this is a while or an assign

	if (AffectsUtil::isWhileStatement(cfg, current)) {
		handleWhileStatement(current, vars);
	}
	if (AffectsUtil::isAssignStatement(cfg, current)) {
		handleAssignStatement(current, vars);
	}
}

void AffectsTSynStmt::handleIfStatement(const stmtNo_t& current, std::set<std::string>& vars) {
	//fork join
	std::set<std::string> then_copy = vars;
	std::set<std::string> else_copy = vars;

	auto nexts = cfg.getPrevof(current);
	stmtNo_t then_end = *(nexts.begin());
	stmtNo_t else_end = *(nexts.rbegin());

	evaluate(-current, then_end, then_copy);
	evaluate(-current, else_end, else_copy);

	then_copy.insert(else_copy.cbegin(), else_copy.cend());
	vars = then_copy;
}

void AffectsTSynStmt::handleWhileStatement(const stmtNo_t& current, std::set<std::string>& vars) {
	// loop until stable
	std::set<std::string> old_vars{};
	std::set<std::string> child_vars{};
	while (vars != old_vars) {
		old_vars = vars;
		child_vars = vars;
		evaluate(current + 1, -current, child_vars);
		vars.insert(child_vars.cbegin(), child_vars.cend());
	}
}

void AffectsTSynStmt::handleAssignStatement(const stmtNo_t& current, std::set<std::string>& vars) {
	auto modifiedVar = AffectsUtil::getVariableModified(facade, current);
	
	// modified var not in collection --> ignore
	if (vars.find(modifiedVar) == vars.cend()) return;

	result.insert(current);
	auto usedVars = AffectsUtil::getVariablesUsed(facade, current);
	vars.erase(modifiedVar);
	vars.insert(usedVars.cbegin(), usedVars.cend());
	return;
}
