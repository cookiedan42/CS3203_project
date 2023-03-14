#include "./GetAssignStatementEntityRequest.h"

GetAssignStatementEntityRequest::GetAssignStatementEntityRequest() : GetEntityRequest(EntityMode::ASSIGNMENT) {};

EntityResults GetAssignStatementEntityRequest::execute(EntityStorage& entityStorage) const {
	StatementEntityStorage storage = entityStorage.getStatementStorage();
	return storage.getAllStmtFromType(ASSIGNMENTTYPE);
}
