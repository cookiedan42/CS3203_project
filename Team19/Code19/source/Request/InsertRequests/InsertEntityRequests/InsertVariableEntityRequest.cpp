#include "./InsertVariableEntityRequest.h"

InsertVariableEntityRequest::InsertVariableEntityRequest(Ent::Variable variableEntity) :
	variableEntity(variableEntity) {}

Ent::Variable InsertVariableEntityRequest::getVariable() const {
	return variableEntity;
}

void InsertVariableEntityRequest::execute(EntityStorage& entityStorage) const {
	entityStorage.insertEntity(variableEntity);
}