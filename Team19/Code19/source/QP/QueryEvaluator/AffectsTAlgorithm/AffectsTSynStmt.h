#pragma once
#include <set>

#include "../../../CFG/Cfg.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsUtil.h"

class AffectsTSynStmt {
    GetFacade& facade;
    const Cfg& cfg;
    std::set<stmtNo_t> result;

public:
    AffectsTSynStmt(GetFacade& facade, const Cfg& cfg);

    RelationshipResults evaluate(const stmtNo_t& end);

private:
    void evaluate( const stmtNo_t& start, const stmtNo_t& end, std::set<std::string>& vars);

    void handleIfStatement(const stmtNo_t& current, std::set<std::string>& vars);
    void handleWhileStatement(const stmtNo_t& current, std::set<std::string>& vars);
    void handleAssignStatement(const stmtNo_t& current, std::set<std::string>& vars);
};
