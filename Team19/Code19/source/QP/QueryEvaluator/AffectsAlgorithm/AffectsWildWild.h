#pragma once

#include "../../../CFG/Cfg.h"
#include "../../../Definitions/Definitions.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsUtil.h"

class AffectsWildWild {
    GetFacade& facade;
    const Cfg& cfg;

public:
    AffectsWildWild(GetFacade& facade, const Cfg& cfg);

    bool evaluate();

private:
    bool evaluate(const stmtNo_t& start, const stmtNo_t& end, LatestModifiedTable& lmt);

    bool handleAssignStatement(stmtNo_t current, LatestModifiedTable& lmt);
    bool handleIfStatement(stmtNo_t current, LatestModifiedTable& lmt);
    bool handleWhileStatement(stmtNo_t current, LatestModifiedTable& lmt);
};
