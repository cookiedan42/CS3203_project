#include "./InsertUsesRequest.h"

InsertUsesRequest::InsertUsesRequest(Rel::Uses usesRel) {
	this->usesRel = usesRel;
}

void InsertUsesRequest::execute(RelationshipStorage& storage) {
	storage.insertRelationship(move(usesRel));
}