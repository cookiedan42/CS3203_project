#include "./GetWhileStatementEntityRequest.h"

GetWhileStatementEntityRequest::GetWhileStatementEntityRequest() : GetEntityRequest(EntityMode::WHILE) {};

EntityResults GetWhileStatementEntityRequest::execute(EntityStorage& entityStorage) const {
	StatementEntityStorage storage = entityStorage.getStatementStorage();
	return storage.getAllStmtFromType(WHILETYPE);
}
