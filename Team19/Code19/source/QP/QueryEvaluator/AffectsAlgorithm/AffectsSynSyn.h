#pragma once
#include "../../../CFG/Cfg.h"
#include "../../../Definitions/Definitions.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsUtil.h"

class AffectsSynSyn {
    GetFacade& facade;
    const Cfg& cfg;
    RelationshipResults results;

public:
    AffectsSynSyn(GetFacade& facade, const Cfg& cfg);

    RelationshipResults evaluate();


private:
    void evaluate(const stmtNo_t& start, const stmtNo_t& end, LatestModifiedTable& lmt);

    void handleAssignStatement(const stmtNo_t& current, LatestModifiedTable& lmt);
    void handleIfStatement(const stmtNo_t& current, LatestModifiedTable& lmt);
    void handleWhileStatement(const stmtNo_t& current, LatestModifiedTable& lmt);

    void addAffectsToResults(const stmtNo_t& affecter, const stmtNo_t& affected);
};
