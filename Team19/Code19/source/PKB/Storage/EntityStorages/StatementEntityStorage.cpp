#include "StatementEntityStorage.h"

StatementEntityStorage::StatementEntityStorage() {
	StatementEntityStorage::possibleStmtTypes = {
		CALLTYPE, PRINTTYPE, ASSIGNMENTTYPE,
			IFTYPE, WHILETYPE, READTYPE
	};
}

// stmtType should be either call/assign/if/else. PROCEDURES ARE NOT STMTS
void StatementEntityStorage::insertStatement(Ent::Statement statement) {

	auto casted = dynamic_cast<Ent::Statement*>(&statement);
	stmtNoRef stmtNo = std::to_string(casted->getStmtNo());
	stmtType stmtType = statement.getName();
	VarOrProc varOrProc = casted->getOptionalArg();

	helperAddToMap(stmtTypeToStmtNoMap, stmtType, stmtNo);

	if (stmtType == Ent::read_stmtType) {
		readVariableSet.insert(varOrProc);
	}
	else if (stmtType == Ent::print_stmtType) {
		printVariableSet.insert(varOrProc);
	}
	else if (stmtType == Ent::call_stmtType) {
		callsProcSet.insert(varOrProc);
		stmtNoToCalledProcMap.emplace(stmtNo, varOrProc);
	}
}


EntityResults StatementEntityStorage::getAllStmtFromType(stmtType stmtType) {

	if (stmtTypeToStmtNoMap.find(stmtType) != stmtTypeToStmtNoMap.end()) {
		//if stmtNo exist
		return stmtTypeToStmtNoMap.at(stmtType);
	}

	return std::unordered_set<std::string>();
}
// stmtType should be either print/call/assign/if/else. PROCEDURES ARE NOT STMTS
EntityResults StatementEntityStorage::getAllStmt() {

	EntityResults answer;
	for (auto& a : possibleStmtTypes) {
		EntityResults check = getAllStmtFromType(a);
		if (!check.empty()) {
			answer.insert(check.begin(), check.end());
		}

	}
	return answer;
}

// Helper functions
void StatementEntityStorage::helperAddToMap(keyStrValueStringSetMap& mapAB, mapKey key, mapVal value) {

	if (mapAB.find(key) != mapAB.end()) {
		//if stmtNo exist
		mapAB.at(key).insert(value);
	}
	else {
		//if stmtNo does not exist
		std::unordered_set<std::string> newSet({ value });
		mapAB.emplace(key, newSet);
	}
}

EntityResults StatementEntityStorage::getAllReadVariables() {
	return readVariableSet;
}

EntityResults StatementEntityStorage::getAllPrintVariables() {
	return printVariableSet;
}

EntityResults StatementEntityStorage::getAllCallsProc() {
	return callsProcSet;
}


EntityResults StatementEntityStorage::getCallsByStmtNo(stmtNoRef stmtNo) {
	if (stmtNoToCalledProcMap.find(stmtNo) != stmtNoToCalledProcMap.end()) {
		return EntityResults({ stmtNoToCalledProcMap.at(stmtNo) });
	}
	return EntityResults({ "" });
}
