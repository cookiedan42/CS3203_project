#include "./InsertModifiesRequest.h"

InsertModifiesRequest::InsertModifiesRequest(Rel::Modifies modifiesRel) {
	this->modifiesRel = modifiesRel;
}


void InsertModifiesRequest::execute(RelationshipStorage& storage) {
	storage.insertRelationship(move(modifiesRel));
}