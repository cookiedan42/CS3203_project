#include "./GetIfStatementEntityRequest.h"

GetIfStatementEntityRequest::GetIfStatementEntityRequest() : GetEntityRequest(EntityMode::IF) {};

EntityResults GetIfStatementEntityRequest::execute(EntityStorage& entityStorage) const {
	StatementEntityStorage storage = entityStorage.getStatementStorage();
	return storage.getAllStmtFromType(IFTYPE);
}
