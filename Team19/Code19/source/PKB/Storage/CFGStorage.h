#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "../../CFG/Cfg.h"
#include "../../TypeAlias/StorageAlias.h"
#include "../../TypeAlias/TypeAlias.h"

class CFGStorage {
	CFGSet CFGSet;
	ProcToCFGMap procToCFGMap;
	StmtNoToCFGMap stmtNoToCFGMap;

public:
	CFGStorage();

	void insertCFG(const Cfg& cfg);

	CFGResults getAllCFG() const;
	CFGResults getCFGByProc(const procName_t& procName) const;
	CFGResults getCFGByStmtNo(const stmtNo_t& stmtNo) const;
};