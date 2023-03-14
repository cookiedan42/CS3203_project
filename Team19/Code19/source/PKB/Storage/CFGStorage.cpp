#include "CFGStorage.h"

CFGStorage::CFGStorage() {}

void CFGStorage::insertCFG(const Cfg& cfg) {
	auto cfg_p = std::make_shared<Cfg>(cfg);
	CFGSet.emplace(cfg_p);

	auto firstStmtNo = cfg.getFirstStmtNo();
	auto lastStmtNo = cfg.getLastStmtNo();
	for (auto i = firstStmtNo; i <= lastStmtNo; i++) stmtNoToCFGMap.emplace(i, cfg_p);

	auto& procName = cfg.getProcName();
	procToCFGMap.emplace(procName, cfg_p);
}

CFGResults CFGStorage::getAllCFG() const {
	return this->CFGSet;
}

CFGResults CFGStorage::getCFGByProc(const procName_t& procName) const {
	CFGResults results;
	try {
		auto& cfg_p = this->procToCFGMap.at(procName);
		results.emplace(cfg_p);
		return results;
	}
	catch (std::out_of_range&) {
		return results;
	}
}

CFGResults CFGStorage::getCFGByStmtNo(const stmtNo_t& stmtNo) const {
	CFGResults results;
	try {
		auto& cfg_p = this->stmtNoToCFGMap.at(stmtNo);
		results.emplace(cfg_p);
		return results;
	}
	catch (std::out_of_range&) {
		return results;
	}
}
