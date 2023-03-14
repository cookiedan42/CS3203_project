#include "GetFacade.h"

GetFacade::GetFacade(PKB& pkb) {
	this->pkb = &pkb;
}

// get entities
EntityResults GetFacade::get(std::shared_ptr<GetEntityRequest> request) {
	return pkb->getEntityManager()->get(request);
}

// get patterns
PatternResults GetFacade::get(std::shared_ptr<GetPatternRequest> request) {
	return pkb->getPatternManager()->get(request);
}

RelationshipResults GetFacade::get(std::shared_ptr<GetRelationshipRequest> request) {
	return pkb->getRelationshipManager()->get(request);
};

CFGResults GetFacade::get(std::shared_ptr<GetCFGRequest> request) {
	return pkb->getRelationshipManager()->get(request);
}