#include "./InsertFollowsRequest.h"

InsertFollowsRequest::InsertFollowsRequest(Rel::Follows followsRelationship) {
	this->followsRelationship = followsRelationship;
}


void InsertFollowsRequest::execute(RelationshipStorage& storage) {
	storage.insertRelationship(move(followsRelationship));

}