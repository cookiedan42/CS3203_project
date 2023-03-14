#pragma once

#include "../PKB/Storage/RelationshipStorage.h"
#include "../CFG/Cfg.h"
#include "../TypeAlias/StorageAlias.h"
#include "../TypeAlias/TypeAlias.h"
#include "Request.h"

enum class CFGMode {
	ALL,
	FILTER_PROC,
	FILTER_STMT_NO,
};

class GetCFGRequest : public Request {
	CFGMode mode;
	procName_t procName;
	stmtNo_t stmtNo;

public:
	GetCFGRequest();
	GetCFGRequest(procName_t procName);
	GetCFGRequest(stmtNo_t stmtNo);

	CFGMode getMode() const;
	procName_t getProcName() const;
	stmtNo_t getStmtNo() const;

	CFGResults execute(RelationshipStorage& storage);
};