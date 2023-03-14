#include "AffectsStmtSyn.h"

AffectsStmtSyn::AffectsStmtSyn(GetFacade& facade, const Cfg& cfg, const stmtNo_t start) :
	facade(facade), cfg(cfg), start(start) {}

RelationshipResults AffectsStmtSyn::evaluate() {
	auto& commonVar = AffectsUtil::getVariableModified(facade, start);

	RelationshipResults results{};
	std::set<stmtNo_t> visited{};
	std::deque<stmtNo_t> stmtQueue{};

	for (const auto& next : cfg.getNextof(start)) stmtQueue.push_back(next);

	while (!stmtQueue.empty()) {
		stmtNo_t current = stmtQueue.front();
		stmtQueue.pop_front();

		if (visited.count(current)) continue;
		visited.insert(current);

		if (AffectsUtil::isAssignStatement(cfg, current)
			&& AffectsUtil::checkStmtUses(facade, current, commonVar)) {
			results.insert({ std::to_string(start), std::to_string(current) });
		}

		// statement modifies the commonVar, this track ends on this stmt
		if (AffectsUtil::isNotContainerStatement(cfg, current) &&
			AffectsUtil::checkStmtModifies(facade, current, commonVar)) continue;

		for (const auto& next : cfg.getNextof(current)) stmtQueue.push_back(next);
	}
	return results;
}
