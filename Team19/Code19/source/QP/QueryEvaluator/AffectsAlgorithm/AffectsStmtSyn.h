#pragma once

#include <deque>
#include <set>

#include "../../../CFG/Cfg.h"
#include "../../../Definitions/Definitions.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsUtil.h"

class AffectsStmtSyn {
	GetFacade facade;
	const Cfg cfg;
	const stmtNo_t start;

public:
	AffectsStmtSyn(GetFacade& facade, const Cfg& cfg, const stmtNo_t start);

	RelationshipResults evaluate();
};