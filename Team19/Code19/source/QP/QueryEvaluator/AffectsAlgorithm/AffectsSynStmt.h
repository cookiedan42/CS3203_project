#pragma once

#include <deque>
#include <set>

#include "../../../CFG/Cfg.h"
#include "../../../Definitions/Definitions.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsUtil.h"

class AffectsSynStmt {
	GetFacade facade;
	const Cfg cfg;
	const stmtNo_t end;

public:
	AffectsSynStmt(GetFacade& facade, const Cfg& cfg, const stmtNo_t end);

	RelationshipResults evaluate();
};