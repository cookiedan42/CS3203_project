#pragma once
#include <map>
#include <set>

#include "../../../CFG/Cfg.h"
#include "../../../PKB/GetFacade.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../AffectsAlgorithm/AffectsSynSyn.h"
#include "../AffectsUtil.h"

class AffectsTSynSyn {
    GetFacade& facade;

public:
    AffectsTSynSyn(GetFacade& facade);

    RelationshipResults evaluate();

private:
    RelationshipResults allPairs(std::map<stmtNo_t, std::set<stmtNo_t>> table);
    std::set<stmtNo_t> allDest(stmtNo_t start, std::map<stmtNo_t, std::set<stmtNo_t>> table);
};
