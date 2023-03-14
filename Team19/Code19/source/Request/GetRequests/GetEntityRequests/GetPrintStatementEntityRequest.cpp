#include "./GetPrintStatementEntityRequest.h"

GetPrintStatementEntityRequest::GetPrintStatementEntityRequest(EntityMode entitymode) : GetEntityRequest(entitymode) {};

EntityResults GetPrintStatementEntityRequest::execute(EntityStorage& entityStorage) const {

	EntityMode entitymode = getEntityMode();
	StatementEntityStorage storage = entityStorage.getStatementStorage();
	switch (entitymode) {
	case EntityMode::PRINT:
		return storage.getAllStmtFromType(PRINTTYPE);
	case EntityMode::PRINTVAR:
		return storage.getAllPrintVariables();
	default:
		return storage.getAllStmtFromType(PRINTTYPE);
	}
}