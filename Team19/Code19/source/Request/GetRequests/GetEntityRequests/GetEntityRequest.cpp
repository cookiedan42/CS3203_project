#include "./GetEntityRequest.h"

GetEntityRequest::GetEntityRequest() {}

GetEntityRequest::GetEntityRequest(EntityMode entitymode, entityRef varExtra) {
	this->entitymode = entitymode;
	this->varExtra = varExtra;
}


EntityMode GetEntityRequest::getEntityMode() const {
	return this->entitymode;
}

stmtNoRef GetEntityRequest::getVarExtra() const {
	return this->varExtra;
}