#include "./GetProcedureEntityRequest.h"

GetProcedureEntityRequest::GetProcedureEntityRequest() : GetEntityRequest(EntityMode::PROCEDURE) {}

EntityResults GetProcedureEntityRequest::execute(EntityStorage& entityStorage) const {
	ProcedureEntityStorage storage = entityStorage.getProcedureStorage();
	return storage.getAllProc();
}
