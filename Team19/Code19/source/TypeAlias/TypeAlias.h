#pragma once
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <utility>
#include "../Definitions/Definitions.h"
#include "../Hash/Hash.h"


//For PKB data structures
typedef std::pair <std::string, std::string> stringPair;
typedef std::unordered_set <stringPair, stringPairHash> stringPairSet;
typedef std::unordered_map<std::string, stringPairSet> intToStringPairMap;
typedef stringPairSet RelationshipResults;
using VarOrProc = std::string;

//For GetRequest objects
typedef std::string Arg;
typedef std::string Arg1TypeString;
typedef std::string RelationshipType;
typedef std::string EntityType;


//Star 
typedef bool HasStar;

// Pattern Data Structures
using stmtNoRef = std::string;
using lhsRef = std::string;
using rhsRef = std::string;
using ptnRef = std::string;
using patternArg = std::string;
using mapKey = std::string;
using mapVal = std::string;
typedef std::pair<stmtNoRef, ptnRef> stmtNoPtnRefPair;
typedef std::unordered_set <stmtNoPtnRefPair, stringPairHash> stmtNoPtnRefPairSet;
using stmtNoToStringMap = std::unordered_map<std::string, std::string>;
using stmtNoVec = std::vector<stmtNoRef>;
using ptnKeyStmtNoVecMap = std::unordered_map<std::string, std::vector<std::string>>;

// Entity Data structures
using varRef = std::string;
using constRef = std::string;
using procRef = std::string;
using stmtType = std::string;
using whileStmtNoRef = std::string;
using ifStmtNoRef = std::string;
using callStmtNoRef = std::string;
using entityRef = std::string;
using EntityResults = std::unordered_set<entityRef>;


using allStmtTypes = std::vector<stmtType>;
using keyStrValueStringSetMap = std::unordered_map<std::string, std::unordered_set<std::string>>;
using keySet = std::unordered_set<mapKey>;
using stmtNoLhsPair = std::pair<stmtNoRef, lhsRef>;


using lhsRef = std::string;
using rhsRef = std::string;
using varRef = std::string;
using patternType = std::string;

using condVarList = std::unordered_set<varRef>;

using listOfWhileStmtNoRef = std::unordered_set<whileStmtNoRef>;
using listOfIfStmtNoRef = std::unordered_set<ifStmtNoRef>;
using listOfCallStmtNoRef = std::unordered_set<callStmtNoRef>;

typedef stringPairSet RelationshipResults;
typedef stmtNoPtnRefPairSet PatternResults;

// SP
using stmtNo_t = int;
using procName_t = std::string;

// CFG
using stmtNoSet_t = std::set<stmtNo_t>;
using cfg_t = std::unordered_map<stmtNo_t, stmtNoSet_t>;
using stmtTypeMap_t = std::unordered_map<stmtNo_t, std::string>;

// Validator
typedef std::unordered_set<std::string> CalledSet;
typedef std::unordered_map<std::string, std::vector<std::string>> CallsGraph;

// For Affects data structure
typedef std::unordered_map<std::string, std::unordered_set<int>> LatestModifiedTable;


// For QueryUtil
typedef std::vector<std::vector<std::string>> TableItems;
typedef std::string ArgString;
typedef std::unordered_set<stringPair, stringPairHash> PKBResults;
typedef std::unordered_set<std::string> SynonymResults;
typedef std::unordered_map<std::pair<ArgType,ArgType>,ArgsType,argTypePairHash>ArgTypePairToArgsTypeMap;


// Optimizer
using SynCountMap = std::unordered_map<std::string, int>;
