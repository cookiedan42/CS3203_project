#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include "../Entity/Entity.h"
#include "../TypeAlias/TypeAlias.h"

/*
Object representation of control flow graph


dummy nodes
if statementNo is k, dummy recombine node is -k
while statementNo is l, last node of children is -l
procedure ends with 0
*/
struct Cfg {
private:
	procName_t procName;
	cfg_t forwardCFG {};
	cfg_t backwardCFG {};
	const stmtNo_t firstStmtNo;
	stmtNo_t lastStmtNo;
	stmtTypeMap_t stmtTypeMap {};
public:
	procName_t getProcName() const { return procName; }
	stmtNo_t getFirstStmtNo() const { return firstStmtNo; }
	stmtNo_t getLastStmtNo() const{ return lastStmtNo; }
public:
	/*
	Construct a Control Flow graph for a procedure
	uses the procedure name and the statement number of the first statement
	*/
	Cfg(procName_t procName, stmtNo_t firstStmtNo) :
		procName(procName),
		firstStmtNo(firstStmtNo),
		lastStmtNo(firstStmtNo) {}
	/*
	Insert new edges into the CFG and update last line as needed
	*/
	void insert(const stmtNo_t& front, const stmtNo_t& back);
	void insert(const stmtNo_t& stmtNo, const std::string stmtType);
private:
	const cfg_t& getForward() const { return forwardCFG; }
	const cfg_t& getBackward() const { return backwardCFG; }
public:
	bool contains(const stmtNo_t& target) const;
	stmtNoSet_t getNextof(const stmtNo_t& target) const;
	stmtNoSet_t getPrevof(const stmtNo_t& target) const;

	// return Next*(int,int)
	bool hasPath(const stmtNo_t& start, const stmtNo_t& end) const;
	bool hasPath_node(const stmtNo_t& start, const stmtNo_t& end) const;

	// Next*(int,syn)
	stmtNoSet_t getNextStar(const stmtNo_t& start) const;
	// Next*(syn,int)
	stmtNoSet_t getPrevStar(const stmtNo_t& end) const;
	// algo for Next*(syn,int) and Next*(int,syn)
	stmtNoSet_t getXStar(const stmtNo_t& end, std::function<stmtNoSet_t(const stmtNo_t&)> filler) const;

	// Next*(int,_)
	bool hasNextStar(const stmtNo_t& start) const;
	// Next*(_,int)
	bool hasPrevStar(const stmtNo_t& end) const;
	// Next*(_,_)
	bool hasNextStarWildWild() const;

	// Next*(syn,_)
	stmtNoSet_t getNextStarSynWild() const;
	// Next*(_,syn)
	stmtNoSet_t getNextStarWildSyn() const;

	// Next*(syn,syn)
	std::vector<std::pair<stmtNo_t, stmtNo_t>> getNextStarSynSyn() const;
	std::set<stmtNo_t> allDest(stmtNo_t start, std::map<stmtNo_t, std::set<stmtNo_t>> table) const;

	// return pairs of strings for PKB to generate Next relationship
	std::vector<std::pair<std::string, std::string>> getAllNext() const;

	std::string getStmtType(stmtNo_t target) const;

	std::vector <stmtNo_t> getAssignStmts() const;

	std::vector<stmtNo_t> Cfg::getPosNextRec(stmtNo_t val)const;

	friend bool operator==(const Cfg& lhs, const Cfg& rhs) {
		return (lhs.procName == rhs.procName)
			&& (lhs.forwardCFG == rhs.forwardCFG)
			&& (lhs.backwardCFG == rhs.backwardCFG)
			&& (lhs.firstStmtNo == rhs.firstStmtNo)
			&& (lhs.lastStmtNo == rhs.lastStmtNo);
	}
	friend bool operator!=(const Cfg& lhs, const Cfg& rhs) {
		return !(lhs == rhs);
	}
};
