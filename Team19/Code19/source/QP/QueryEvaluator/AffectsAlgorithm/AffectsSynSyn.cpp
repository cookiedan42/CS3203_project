#include "AffectsSynSyn.h"

AffectsSynSyn::AffectsSynSyn(GetFacade& facade, const Cfg& cfg) : facade(facade), cfg(cfg) {}


RelationshipResults AffectsSynSyn::evaluate() {
	LatestModifiedTable lmt{};
	evaluate(cfg.getFirstStmtNo(), PROC_END, lmt);

	return results;
}

void AffectsSynSyn::evaluate(const stmtNo_t& start, const stmtNo_t& end, LatestModifiedTable& lmt) {
	stmtNo_t current = start;
	
	while (current != end) {
		if (AffectsUtil::isAssignStatement(cfg, current)) {
			handleAssignStatement(current, lmt);
			current = *cfg.getNextof(current).begin();
			continue;
		}

		if (AffectsUtil::isIfStatement(cfg, current)) {
			handleIfStatement(current, lmt);
			stmtNo_t dummyCombineNode = -current;
			current = *cfg.getNextof(dummyCombineNode).begin();
			continue;
		}

		if (AffectsUtil::isWhileStatement(cfg, current)) {
			handleWhileStatement(current, lmt);
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
}

void AffectsSynSyn::handleAssignStatement(const stmtNo_t& current, LatestModifiedTable& lmt) {
	// if current uses anything, it is affected
	auto usedVars = AffectsUtil::getVariablesUsed(facade, current);
	for (const auto& usedVar : usedVars) {
		if (!lmt.count(usedVar)) continue;

		for (const auto& affecting : lmt.at(usedVar)) {
			addAffectsToResults(affecting, current);
		}
	}

	// Add variable modified by current statement to the LMT.
	AffectsUtil::addToLMT(facade, current, lmt);
}

void AffectsSynSyn::handleIfStatement(const stmtNo_t& current, LatestModifiedTable& lmt) {
	auto nexts = cfg.getNextof(current);

	stmtNo_t dummyCombineNode = -current;
	LatestModifiedTable lmtIf = lmt;
	LatestModifiedTable lmtElse = lmt;

	evaluate(*nexts.begin(), dummyCombineNode, lmtIf);
	evaluate(*nexts.rbegin(), dummyCombineNode, lmtElse);
	
	lmt = AffectsUtil::mergeLMTs(lmtIf, lmtElse);
}

void AffectsSynSyn::handleWhileStatement(const stmtNo_t& current, LatestModifiedTable& lmt) {
	LatestModifiedTable lmtBefore = lmt;

	// First statement of children have to be current + 1
	stmtNo_t nextNode = current + 1;
	stmtNo_t dummyLastNode = -current;

	evaluate(nextNode, dummyLastNode, lmt);
	lmt = AffectsUtil::mergeLMTs(lmtBefore, lmt);

	while (lmtBefore != lmt) {
		lmtBefore = lmt;
		evaluate(nextNode, dummyLastNode, lmt);
		lmt = AffectsUtil::mergeLMTs(lmtBefore, lmt);
	}
}

void AffectsSynSyn::addAffectsToResults(const stmtNo_t& affecter, const stmtNo_t& affected) {
	results.insert({ to_string(affecter), to_string(affected) });
}

