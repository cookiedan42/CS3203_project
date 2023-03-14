#include "./InsertStatementEntityRequest.h"

InsertStatementEntityRequest::InsertStatementEntityRequest(Ent::Statement statementEntity) :
	statementEntity(statementEntity) {}

Ent::Statement InsertStatementEntityRequest::getStatement() const {
	return statementEntity;
}

void InsertStatementEntityRequest::execute(EntityStorage& entityStorage) const {
	entityStorage.insertEntity(statementEntity);
}