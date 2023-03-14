#include "WhilePatternStorage.h"

WhilePatternStorage::WhilePatternStorage() {}

void WhilePatternStorage::insertPattern(Ptn::WhilePtn pattern) {
	stmtNoRef stmtNo = pattern.getStmtNo();
	std::unordered_set<std::string> condVarSet = pattern.getVarSet();
	for (auto& var : condVarSet) {
		helperAddToMap(whileVarStmtNoVecOneToManyMap, var, stmtNo);
		allWhileStmtNoPtnPair.insert(stmtNoPtnRefPair(stmtNo, var));
	}
}

stmtNoPtnRefPairSet WhilePatternStorage::getAllWhilePtn() {											// supports while_Syn( _ , _ ) and while_Syn( Syn , _ )
	return allWhileStmtNoPtnPair;
}

stmtNoPtnRefPairSet WhilePatternStorage::getWhileByArg1(patternArg arg) {							// supports while_Syn( "Quote refering to varName in conditional" , _ )
	stmtNoPtnRefPairSet ans;
	if (whileVarStmtNoVecOneToManyMap.find(arg) != whileVarStmtNoVecOneToManyMap.end()) {		// if arg1(varName) exact copy is a key
		for (auto& stmtNo : whileVarStmtNoVecOneToManyMap.at(arg)) {							// for all statment numbers that has arg1 as lhs
			ans.insert(stmtNoPtnRefPair(stmtNo, arg));										// push a pair describing <stmtno, whilePtn> 
		}
	}
	return ans;
}

void WhilePatternStorage::helperAddToMap(ptnKeyStmtNoVecMap& mapAB, mapKey key, mapVal stmtNo) {

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