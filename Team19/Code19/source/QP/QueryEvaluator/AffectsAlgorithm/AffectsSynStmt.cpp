#include "AffectsSynStmt.h"

AffectsSynStmt::AffectsSynStmt(GetFacade& facade, const Cfg& cfg, const stmtNo_t end) :
	facade(facade), cfg(cfg), end(end) {}

RelationshipResults AffectsSynStmt::evaluate() {
	RelationshipResults results;

	auto variables = AffectsUtil::getVariablesUsed(facade, end);
	for (auto& commonVar : variables) {
		std::set<stmtNo_t> visited{};
		std::deque<stmtNo_t> stmtQueue{};

		//do the start checking
		for (const auto& prev : cfg.getPrevof(end)) stmtQueue.push_back(prev);

		while (!stmtQueue.empty()) {
			stmtNo_t current = stmtQueue.front();
			stmtQueue.pop_front();

			if (visited.count(current)) continue;
			visited.insert(current);

			// assign statement modifies the commonVar, this track terminates
			if (AffectsUtil::isAssignStatement(cfg, current)
				&& AffectsUtil::checkStmtModifies(facade, current, commonVar)) {
				results.insert({ std::to_string(current), std::to_string(end) });
				continue;
			}

			// read/call statement modifies the commonVar, this track terminates
			if (AffectsUtil::isNotContainerStatement(cfg, current) &&
				AffectsUtil::checkStmtModifies(facade, current, commonVar)) continue;

			for (const auto& prev : cfg.getPrevof(current)) stmtQueue.push_back(prev);
		}
	}
	return results;
}