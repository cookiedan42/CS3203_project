#include "./InsertProcedureEntityRequest.h"

InsertProcedureEntityRequest::InsertProcedureEntityRequest(Ent::Procedure  procedureEntity) :
	procedureEntity(procedureEntity) {}

Ent::Procedure InsertProcedureEntityRequest::getProcedure() const {
	return procedureEntity;
}

void InsertProcedureEntityRequest::execute(EntityStorage& entityStorage) const {
	entityStorage.insertEntity(procedureEntity);
}
