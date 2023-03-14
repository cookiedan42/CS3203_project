#include "./InsertParentRequest.h"

InsertParentRequest::InsertParentRequest(Rel::Parent parentRel) {
	this->parentRel = parentRel;
}


void InsertParentRequest::execute(RelationshipStorage& storage) {
	storage.insertRelationship(move(parentRel));

}