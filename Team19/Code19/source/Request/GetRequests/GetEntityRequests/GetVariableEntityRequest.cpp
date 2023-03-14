#include "./GetVariableEntityRequest.h"


GetVariableEntityRequest::GetVariableEntityRequest() : GetEntityRequest(EntityMode::VARIABLE) {}

EntityResults GetVariableEntityRequest::execute(EntityStorage& entityStorage) const {
	VariableEntityStorage storage = entityStorage.getVariableStorage();
	return storage.getAllVar();
}