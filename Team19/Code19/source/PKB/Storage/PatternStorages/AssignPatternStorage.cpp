#include "AssignPatternStorage.h"

AssignPatternStorage::AssignPatternStorage() {}

void AssignPatternStorage::insertPattern(Ptn::AssignPtn pattern) {
	stmtNoRef stmtNo = pattern.getStmtNo();
	patternType type = pattern.getType();
	lhsRef lhs = pattern.getLhs();
	rhsRef inOrder = pattern.getInOrderRHS();

	if (assignStmtNoPatternOneToOneMap.find(stmtNo) == assignStmtNoPatternOneToOneMap.end()) {	// unsures 1 to 1 relationship between stmtNoXrhs and stmtNoXlhs
		helperAddToMap(assignRhsInOrderStmtNoOneToManyMap, inOrder, stmtNo);					// inordered is a flattened tree
		helperAddToMap(assignLhsStmtNoOneToManyMap, lhs, stmtNo);								// lhs can be referenced in multiple stmtno 
		allAssignPair.insert(pattern.getStmtNoLhsPair());									// push a pair describing <stmtno, lhs> 
		assignStmtNoPatternOneToOneMap.emplace(stmtNo, pattern);								// shld be inserted last cos there is a check @helperAddToMap
	}
}


stmtNoPtnRefPairSet AssignPatternStorage::getAllAssign() {
	return allAssignPair;
}


PatternResults AssignPatternStorage::getAssignByArg1(patternArg arg1) {
	PatternResults ans;
	if (assignLhsStmtNoOneToManyMap.find(arg1) != assignLhsStmtNoOneToManyMap.end()) {			// if arg1(lhs) exact copy is a key
		for (auto& stmtNo : assignLhsStmtNoOneToManyMap.at(arg1)) {								// for all statment numbers that has arg1 as lhs
			ans.insert(assignStmtNoPatternOneToOneMap.at(stmtNo).getStmtNoLhsPair());		// push a pair describing <stmtno, lhs> 
		}
	}
	return ans;
}

PatternResults AssignPatternStorage::getAssignByCompleteArg2(patternArg preOrder, patternArg inOrder) {
	PatternResults ans;
	if (assignRhsInOrderStmtNoOneToManyMap.find(inOrder) != assignRhsInOrderStmtNoOneToManyMap.end()) {			// if arg2(rhs) exact copy is a key
		for (auto& stmtNo : assignRhsInOrderStmtNoOneToManyMap.at(inOrder)) {								// for all statement numbers that has arg2 as rhs
			if (assignStmtNoPatternOneToOneMap.at(stmtNo).getPreOrderRHS() == preOrder) {	//if also same preorder
				ans.insert(assignStmtNoPatternOneToOneMap.at(stmtNo).getStmtNoLhsPair());									// push a pair describing <stmtno, lhs> 
			}
		}
	}
	return ans;
}

PatternResults AssignPatternStorage::getAssignBySubArg2(patternArg preOrder, patternArg inOrder) {
	PatternResults ans;
	for (auto& a : assignRhsInOrderStmtNoOneToManyMap) {																			// for all possible rhs
		if (a.first.find(inOrder) != std::string::npos) {																		// if the string is part of Inorder
			for (auto& stmtNo : assignRhsInOrderStmtNoOneToManyMap.at(a.first)) {													// for those that are part of Inorder
				if (assignStmtNoPatternOneToOneMap.at(stmtNo).getPreOrderRHS().find(preOrder) != std::string::npos) {	//if also part of preorder
					ans.insert(assignStmtNoPatternOneToOneMap.at(stmtNo).getStmtNoLhsPair());									// push a pair describing <stmtno, lhs> 
				}
			}
		}
	}
	return ans;
}

PatternResults AssignPatternStorage::getAssignByArg1CompleteArg2(patternArg arg1, patternArg preOrder, patternArg inOrder) {
	PatternResults ans;
	if (assignLhsStmtNoOneToManyMap.find(arg1) != assignLhsStmtNoOneToManyMap.end()) {			// If arg1(lhs) exact copy is a key
		for (auto& stmtNo : assignLhsStmtNoOneToManyMap.at(arg1)) {								// for all statment numbers that has arg1 as lhs
			if (assignStmtNoPatternOneToOneMap.at(stmtNo).getInOrderRHS() == inOrder &&
				assignStmtNoPatternOneToOneMap.at(stmtNo).getPreOrderRHS() == preOrder) {					// if its rhs also same as arg2
				ans.insert(assignStmtNoPatternOneToOneMap.at(stmtNo).getStmtNoLhsPair());	// push a pair describing <stmtno, lhs> 
			}
		}
	}
	return ans;
}

PatternResults AssignPatternStorage::getAssignByArg1SubArg2(patternArg arg1, patternArg preOrder, patternArg inOrder) { // **** will need to modify once AST is introduced
	PatternResults ans;
	if (assignLhsStmtNoOneToManyMap.find(arg1) != assignLhsStmtNoOneToManyMap.end()) {								// If arg1(lhs) exact copy is a key
		for (auto& stmtNo : assignLhsStmtNoOneToManyMap.at(arg1)) {													// for all statment numbers that has arg1 as lhs
			if (assignStmtNoPatternOneToOneMap.at(stmtNo).getInOrderRHS().find(inOrder) != std::string::npos &&
				assignStmtNoPatternOneToOneMap.at(stmtNo).getPreOrderRHS().find(preOrder) != std::string::npos) {	// if its rhs contains arg2 as a substring (Milestone1 sufficient)
				ans.insert(assignStmtNoPatternOneToOneMap.at(stmtNo).getStmtNoLhsPair());						// push a pair describing <stmtno, lhs> 
			}
		}
	}
	return ans;
}


// Helper functions
void AssignPatternStorage::helperAddToMap(ptnKeyStmtNoVecMap& mapAB, mapKey key, mapVal stmtNo) {

	if (assignStmtNoPatternOneToOneMap.find(stmtNo) != assignStmtNoPatternOneToOneMap.end()) { // statment number alrdy populated
		return;
	}

	if (mapAB.find(key) == mapAB.end()) {
		//if stmtNo does not exist
		std::vector<std::string> newVec;
		newVec.push_back(stmtNo);
		mapAB.emplace(key, newVec);
	}
	else {
		mapAB.at(key).push_back(stmtNo);
	}
}