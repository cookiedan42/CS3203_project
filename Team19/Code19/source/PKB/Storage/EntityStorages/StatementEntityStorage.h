#pragma once

#include "../Storage.h"


class StatementEntityStorage : public Storage{
public:
	StatementEntityStorage();

	void insertStatement(Ent::Statement);

	EntityResults getAllStmtFromType(stmtType);
	EntityResults getAllStmt();
	EntityResults getAllReadVariables();
	EntityResults getAllPrintVariables();
	EntityResults getAllCallsProc();
	EntityResults getCallsByStmtNo(stmtNoRef);


private:
	allStmtTypes possibleStmtTypes;
	// helper functions
	void helperAddToMap(keyStrValueStringSetMap&, mapKey, mapVal);

	keyStrValueStringSetMap stmtTypeToStmtNoMap;
	keySet readVariableSet;
	keySet printVariableSet;
	keySet callsProcSet;

	stmtNoToStringMap stmtNoToCalledProcMap;
};