#include "AffectsStmtWild.h"

AffectsStmtWild::AffectsStmtWild(GetFacade& facade, const Cfg& cfg, const stmtNo_t start) :
	facade(facade), cfg(cfg), start(start) {}

bool AffectsStmtWild::evaluate() {
	auto commonVar = AffectsUtil::getVariableModified(facade, start);

	std::set<stmtNo_t> visited{};
	std::deque<stmtNo_t> stmtQueue{};

	for (const auto& next : cfg.getNextof(start)) stmtQueue.push_back(next);

	while (!stmtQueue.empty()) {
		stmtNo_t current = stmtQueue.front();
		stmtQueue.pop_front();

		if (visited.count(current)) continue;
		visited.insert(current);


		if (AffectsUtil::isAssignStatement(cfg, current) &&
			AffectsUtil::checkStmtUses(facade, current, commonVar)) return true;

		if (AffectsUtil::isNotContainerStatement(cfg, current) &&
			AffectsUtil::checkStmtModifies(facade, current, commonVar)) continue;

		for (const auto& next : cfg.getNextof(current)) stmtQueue.push_back(next);
	}
	return false;
}