#include "AffectsWildWild.h"

AffectsWildWild::AffectsWildWild(GetFacade& facade, const Cfg& cfg) : facade(facade), cfg(cfg) {}

bool AffectsWildWild::evaluate(){
	LatestModifiedTable lmt{};
	return evaluate(cfg.getFirstStmtNo(), 0, lmt);
}

bool AffectsWildWild::evaluate(const stmtNo_t& start, const stmtNo_t& end, LatestModifiedTable& lmt){
	
	auto current = start;

	while (current != end) {
		if (AffectsUtil::isAssignStatement(cfg, current)) {
			bool hasAffects = handleAssignStatement(current, lmt);
			if (hasAffects) return true;

			current = *cfg.getNextof(current).begin();
			continue;
		}

		if (AffectsUtil::isIfStatement(cfg, current)) {
			bool hasAffects = handleIfStatement(current, lmt);
			if (hasAffects) return true;

			stmtNo_t dummyCombineNode = -current;
			current = *cfg.getNextof(dummyCombineNode).begin();
			continue;
		}

		if (AffectsUtil::isWhileStatement(cfg, current)) {
			bool hasAffects = handleWhileStatement(current, lmt);
			if (hasAffects) return true;

			auto nexts = cfg.getNextof(current);
			nexts.erase(current + 1); // remove child node to break out of while-loop
			current = *(nexts.cbegin());

			continue;
		}

		// If code reaches here, current statement is Read/Print/Call
		// Find any variable it modifies, and erase it from LMT
		auto modifiedVars = AffectsUtil::getAllVariablesModified(facade, current);
		for (const auto& modifiedVar : modifiedVars) lmt.erase(modifiedVar);

		current = *cfg.getNextof(current).begin();
	}

	return false;
}

bool AffectsWildWild::handleAssignStatement(stmtNo_t current, LatestModifiedTable& lmt){
	auto usedVars = AffectsUtil::getVariablesUsed(facade, current);
	for (const auto& usedVar : usedVars) {
		if (lmt.count(usedVar)) return true;
	}

	// Add variable modified by current statement to the LMT.
	AffectsUtil::addToLMT(facade, current, lmt);

	return false;
}

bool AffectsWildWild::handleIfStatement(stmtNo_t current, LatestModifiedTable& lmt){
	auto nexts = cfg.getNextof(current);

	stmtNo_t dummyCombineNode = -current;
	LatestModifiedTable lmtIf = lmt;
	LatestModifiedTable lmtElse = lmt;

	bool affects = evaluate(*nexts.begin(), dummyCombineNode, lmtIf);
	if (affects) return true;

	affects = evaluate(*nexts.rbegin(), dummyCombineNode, lmtElse);
	if (affects) return true;

	lmt = AffectsUtil::mergeLMTs(lmtIf, lmtElse);

	return false;
}

bool AffectsWildWild::handleWhileStatement(stmtNo_t current, LatestModifiedTable& lmt){
	LatestModifiedTable lmtBefore = lmt;

	// First statement of children have to be current + 1
	stmtNo_t next = current + 1;
	stmtNo_t dummyLastNode = -current;

	bool affects = evaluate(next, dummyLastNode, lmt);
	if (affects) return true;
	lmt = AffectsUtil::mergeLMTs(lmtBefore, lmt);

	while (lmtBefore != lmt) {
		lmtBefore = lmt;
		affects = evaluate(next, dummyLastNode, lmt);
		if (affects) return true;
		lmt = AffectsUtil::mergeLMTs(lmtBefore, lmt);
	}

	return false;
}

