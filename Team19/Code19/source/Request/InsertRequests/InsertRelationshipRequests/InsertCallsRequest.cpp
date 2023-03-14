#include "./InsertCallsRequest.h"

InsertCallsRequest::InsertCallsRequest(Rel::Calls callsRelationship) {
	this->callsRelationship = callsRelationship;
}


void InsertCallsRequest::execute(RelationshipStorage& storage) {
	storage.insertRelationship(move(callsRelationship));
}