#include "AffectsStmtStmt.h"

AffectsStmtStmt::AffectsStmtStmt(GetFacade& facade, const Cfg& cfg, const stmtNo_t start, const stmtNo_t end) :
	facade(facade), cfg(cfg), start(start), end(end) {}

bool AffectsStmtStmt::evaluate() {
	auto var = AffectsUtil::getModifiedAndUsed(facade, start, end);
	if (!var.has_value()) return false;

	auto& commonVar = var.value();

	std::set<stmtNo_t> visited{};
	std::deque<stmtNo_t> stmtQueue{};

	//do the start checking
	for (const auto& next : cfg.getNextof(start)) stmtQueue.push_back(next);

	while (!stmtQueue.empty()) {
		stmtNo_t current = stmtQueue.front();
		stmtQueue.pop_front();

		if (current == PROC_END) continue; // end of proc 
		if (visited.count(current)) continue;
		visited.insert(current);

		// end uses commonVar established at start
		if (current == end) return true;

		// statement modifies the commonVar, this track is not desirable
		// also check that statement is not a container stmt
		if (AffectsUtil::isNotContainerStatement(cfg, current) &&
			AffectsUtil::checkStmtModifies(facade, current, commonVar)) continue;

		for (const auto& next : cfg.getNextof(current)) stmtQueue.push_back(next);
	}
	return false;
}