#pragma once

#include <string>
#include <optional>

#include "../../CFG/Cfg.h"
#include "../../PKB/GetFacade.h"
#include "../../Request/GetCFGRequest.h"
#include "../../Request/GetRequests/GetRelationshipRequests/GetModifiesRequest.h"
#include "../../Request/GetRequests/GetRelationshipRequests/GetUsesRequest.h"
#include "../../TypeAlias/TypeAlias.h"

using namespace std;

class AffectsUtil {
public:
    static bool isAssignStatement(const Cfg& cfg, const stmtNo_t& stmtNo);
    static bool isIfStatement(const Cfg& cfg, const stmtNo_t& stmtNo);
    static bool isWhileStatement(const Cfg& cfg, const stmtNo_t& stmtNo);
    static bool isNotContainerStatement(Cfg cfg, stmtNo_t current);

    static void addToLMT(GetFacade& facade, const stmtNo_t& current, LatestModifiedTable& lmt);

    static LatestModifiedTable mergeLMTs(LatestModifiedTable& lmt1, LatestModifiedTable& lmt2);

    /*
        Checks if both statements are in the same procedure/CFG, and return the CFG if true
    */
    static optional<Cfg> getCommonCFG(GetFacade& facade, const stmtNo_t& start, const stmtNo_t& end);

    /*
        Get the CFG that contains the stmtNo, if it exists.
    */
    static optional<Cfg> getCFGByStmtNo(GetFacade& facade, const stmtNo_t& stmtNo);

    /*
        Get all CFGs from PKB
    */
    static CFGResults getAllCFGs(GetFacade& facade);

    /*
        Get the variable modified by arg1 and used by arg2, if the variable exists
    */
    static optional<string> getModifiedAndUsed(GetFacade& facade, const stmtNo_t& arg1, const stmtNo_t& arg2);

    static unordered_set<string> getVariablesUsed(GetFacade& facade, const stmtNo_t& stmtNo);

    static string getVariableModified(GetFacade& facade, const stmtNo_t& stmtNo);

    static unordered_set<string> getAllVariablesModified(GetFacade& facade, const stmtNo_t& stmtNo);

    static bool checkStmtUses(GetFacade& facade, const stmtNo_t& current, const std::string& commonVar);

    static bool checkStmtModifies(GetFacade& facade, const stmtNo_t& current, const std::string& commonVar);
};