#include "./GetConstantEntityRequest.h"

GetConstantEntityRequest::GetConstantEntityRequest() : GetEntityRequest(EntityMode::CONSTANT) {}


EntityResults GetConstantEntityRequest::execute(EntityStorage& entityStorage) const {
	ConstantEntityStorage storage = entityStorage.getConstantStorage();
	return storage.getAllConstant();
}
