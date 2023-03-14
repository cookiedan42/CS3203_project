#pragma once

#include <deque>
#include <set>

#include "../../../CFG/Cfg.h"
#include "../../../Definitions/Definitions.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsUtil.h"

class AffectsWildStmt {
	GetFacade facade;
	const Cfg cfg;
	const stmtNo_t end;

public:
	AffectsWildStmt(GetFacade& facade, const Cfg& cfg, const stmtNo_t end);

	bool evaluate();
};