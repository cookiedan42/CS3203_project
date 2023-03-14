#include "./GetStatementEntityRequest.h"

GetStatementEntityRequest::GetStatementEntityRequest() : GetEntityRequest() {};
GetStatementEntityRequest::GetStatementEntityRequest(EntityMode entitymode, entityRef VarExtra) : GetEntityRequest(entitymode, VarExtra) {};

EntityResults GetStatementEntityRequest::execute(EntityStorage& entityStorage) const {
	StatementEntityStorage storage = entityStorage.getStatementStorage();
	return storage.getAllStmt();
}