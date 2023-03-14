#include "./GetCallStatementEntityRequest.h"

GetCallStatementEntityRequest::GetCallStatementEntityRequest(EntityMode entitymode, stmtNoRef VarExtra) : GetEntityRequest(entitymode, VarExtra) {};

EntityResults GetCallStatementEntityRequest::execute(EntityStorage& entityStorage) const {

	EntityMode entitymode = getEntityMode();
	StatementEntityStorage storage = entityStorage.getStatementStorage();
	switch (entitymode) {
	case EntityMode::CALL:
		return storage.getAllStmtFromType(CALLTYPE);
	case EntityMode::CALLBYSTMT:
		return storage.getCallsByStmtNo(getVarExtra());
	case EntityMode::CALLSPROC:
		return storage.getAllCallsProc();
	default:
		return storage.getAllStmtFromType(CALLTYPE);
	}
}
