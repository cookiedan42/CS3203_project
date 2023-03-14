#include "./GetReadStatementEntityRequest.h"

GetReadStatementEntityRequest::GetReadStatementEntityRequest(EntityMode entitymode) : GetEntityRequest(entitymode) {};

EntityResults GetReadStatementEntityRequest::execute(EntityStorage& entityStorage) const {

	EntityMode entitymode = getEntityMode();
	StatementEntityStorage storage = entityStorage.getStatementStorage();
	switch (entitymode) {
	case EntityMode::READ:
		return storage.getAllStmtFromType(READTYPE);
	case EntityMode::READVAR:
		return storage.getAllReadVariables();
	default:
		return storage.getAllStmtFromType(READTYPE);
	}
}