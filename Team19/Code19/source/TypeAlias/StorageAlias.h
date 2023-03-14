#pragma once
#include<memory>
#include <unordered_set>

#include "../CFG/Cfg.h"

typedef std::unordered_set<std::shared_ptr<Cfg>> CFGResults;
typedef std::unordered_set<std::shared_ptr<Cfg>> CFGSet;
typedef std::unordered_map<procName_t, std::shared_ptr<Cfg>> ProcToCFGMap;
typedef std::unordered_map<stmtNo_t, std::shared_ptr<Cfg>> StmtNoToCFGMap;
