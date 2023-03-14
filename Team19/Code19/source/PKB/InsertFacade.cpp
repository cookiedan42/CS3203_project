#include "InsertFacade.h"

InsertFacade::InsertFacade(PKB& pkb) {
	this->pkb = &pkb;
}


void InsertFacade::insert(std::shared_ptr<InsertEntityRequest> request) {
	pkb->getEntityManager()->insert(request);
}

void InsertFacade::insert(std::shared_ptr<InsertPatternRequest> request) {
	pkb->getPatternManager()->insert(request);
}

void InsertFacade::insert(std::shared_ptr<InsertRelationshipRequest> request) {
	pkb->getRelationshipManager()->insert(request);
}

void InsertFacade::insertCFG(Cfg cfg) {
	pkb->getRelationshipManager()->insertCFG(cfg);
}

void InsertFacade::generateSecondaryData() {
	pkb->getRelationshipManager()->generateStarData();
	pkb->getRelationshipManager()->generateNextData();
}