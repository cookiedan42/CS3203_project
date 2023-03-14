#pragma once
#include "../../../CFG/Cfg.h"
#include "../../../Definitions/Definitions.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsUtil.h"

class AffectsTStmtStmt {
    GetFacade& facade;
    const Cfg& cfg;
    const stmtNo_t target;

    bool result = false;

public:
    AffectsTStmtStmt(GetFacade& facade, const Cfg& cfg, stmtNo_t target);

    bool evaluate(const stmtNo_t& start, const stmtNo_t& end);

private:
    void evaluate(const stmtNo_t& start, const stmtNo_t& end, std::set<std::string>& vars);

    void handleIfStatement(const stmtNo_t& current, std::set<std::string>& vars);
    void handleWhileStatement(const stmtNo_t& current, std::set<std::string>& vars);
    void handleAssignStatement(const stmtNo_t& current, std::set<std::string>& vars);
};
