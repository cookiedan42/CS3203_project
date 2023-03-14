#include "AffectsWildStmt.h"

AffectsWildStmt::AffectsWildStmt(GetFacade& facade, const Cfg& cfg, const stmtNo_t end) :
	facade(facade), cfg(cfg), end(end) {}

bool AffectsWildStmt::evaluate() {
	auto commonVariables = AffectsUtil::getVariablesUsed(facade, end);

	for (const auto& commonVar : commonVariables) {
		// start at end
		// climb up the tree 
		// terminate && add to result if modifies
		std::set<stmtNo_t> visited{};
		std::deque<stmtNo_t> stmtQueue{};

		for (const auto& prev : cfg.getPrevof(end)) stmtQueue.push_back(prev);

		while (!stmtQueue.empty()) {
			stmtNo_t current = stmtQueue.front();
			stmtQueue.pop_front();

			if (visited.count(current)) continue;
			visited.insert(current);

			// assign statement modifies the commonVar, this track terminates
			if (AffectsUtil::isAssignStatement(cfg, current) &&
				AffectsUtil::checkStmtModifies(facade, current, commonVar)) return true;

			// read/call statement modifies the commonVar, this track terminates
			if (AffectsUtil::isNotContainerStatement(cfg, current) &&
				AffectsUtil::checkStmtModifies(facade, current, commonVar)) continue;

			for (const auto& prev : cfg.getPrevof(current)) stmtQueue.push_back(prev);
		}
	}
	return false;
}