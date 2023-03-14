#include "./InsertConstantEntityRequest.h"

InsertConstantEntityRequest::InsertConstantEntityRequest(Ent::Constant constantEntity) :
	constantEntity(constantEntity) {}

Ent::Constant InsertConstantEntityRequest::getConstant() const {
	return constantEntity;
}

void InsertConstantEntityRequest::execute(EntityStorage& entityStorage) const {
	entityStorage.insertEntity(constantEntity);
}