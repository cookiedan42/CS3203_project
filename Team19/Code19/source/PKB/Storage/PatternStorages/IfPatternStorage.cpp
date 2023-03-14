#include "IfPatternStorage.h"

IfPatternStorage::IfPatternStorage() {}

void IfPatternStorage::insertPattern(Ptn::IfPtn pattern) {
	stmtNoRef stmtNo = pattern.getStmtNo();
	std::unordered_set<std::string> condVarSet = pattern.getVarSet();
	for (auto& var : condVarSet) {
		helperAddToMap(ifVarStmtNoVecOneToManyMap, var, stmtNo);
		allIfStmtNoPtnPair.insert(stmtNoPtnRefPair(stmtNo, var));							// no time constraint when storing 
	}
}


stmtNoPtnRefPairSet IfPatternStorage::getAllIfPtn() {												// supports if_Syn( _ , _ ) and if_Syn( Syn , _ )
	return allIfStmtNoPtnPair;
}


PatternResults IfPatternStorage::getIfByArg1(patternArg arg) {								// supports if_Syn( "Quote refering to varName in conditional" , _ )
	PatternResults ans;
	if (ifVarStmtNoVecOneToManyMap.find(arg) != ifVarStmtNoVecOneToManyMap.end()) {				// if arg1(varName) exact copy is a key
		for (auto& stmtNo : ifVarStmtNoVecOneToManyMap.at(arg)) {								// for all statment numbers that has arg1 as lhs
			ans.insert(stmtNoPtnRefPair(stmtNo, arg));										// push a pair describing <stmtno, ifPtn> 
		}
	}
	return ans;
}

// Helper functions
void IfPatternStorage::helperAddToMap(ptnKeyStmtNoVecMap& mapAB, mapKey key, mapVal stmtNo) {

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