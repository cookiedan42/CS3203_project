#include "./GetRelationshipRequest.h"

GetRelationshipRequest::GetRelationshipRequest(Arg arg1, Arg arg2, Mode mode) :
	arg1(arg1), arg2(arg2), mode(mode) {}

GetRelationshipRequest::GetRelationshipRequest(Arg arg, Mode mode) : mode(mode) {
	if (mode == Mode::FILTER_ARG_1) {
		this->arg1 = arg;
		this->arg2 = EMPTY_ARG;
	}

	if (mode == Mode::FILTER_ARG_2) {
		this->arg1 = EMPTY_ARG;
		this->arg2 = arg;
	}
}

GetRelationshipRequest::GetRelationshipRequest(Mode mode) : mode(mode) {
	this->arg1 = EMPTY_ARG;
	this->arg2 = EMPTY_ARG;
}

Mode GetRelationshipRequest::getMode() const {
	return this->mode;
}

Arg GetRelationshipRequest::getArg1() const {
	return this->arg1;
}

Arg GetRelationshipRequest::getArg2() const {
	return this->arg2;
}

RelationshipResults GetRelationshipRequest::execute(RelationshipStorage& storage) {
	return RelationshipResults();
}
