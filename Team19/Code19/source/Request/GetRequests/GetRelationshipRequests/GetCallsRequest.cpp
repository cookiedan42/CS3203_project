#include "./GetCallsRequest.h"

GetCallsRequest::GetCallsRequest(Arg arg1, Arg arg2, Mode mode, HasStar star) :
	GetRelationshipRequest(arg1, arg2, mode), hasStar(star) {}

GetCallsRequest::GetCallsRequest(Arg arg, Mode mode, HasStar star) :
	GetRelationshipRequest(arg, mode), hasStar(star) {}


GetCallsRequest::GetCallsRequest(Mode mode, HasStar star) :
	GetRelationshipRequest(mode), hasStar(star) {}

HasStar GetCallsRequest::getHasStar() const {
	return this->hasStar;
}

RelationshipResults GetCallsRequest::execute(RelationshipStorage& storage) {
	Mode mode = getMode();
	//As long as we know got star, can just put star as parameters (checking if has star will be done internally)
	HasStar star = getHasStar();
	CallsStorage callStorage = storage.getCallsStorage();
	switch (mode) {
	case Mode::FILTER_ARG1_ARG2:
	{
		Arg arg1 = getArg1();
		Arg arg2 = getArg2();
		return callStorage.getRelationshipByArg1AndArg2(arg1, arg2, star);
	}
	case Mode::FILTER_ARG_1:
	{
		Arg arg1 = getArg1();
		return callStorage.getRelationshipByArg1(arg1, star);
	}
	case Mode::FILTER_ARG_2:
	{
		Arg arg2 = getArg2();
		return callStorage.getRelationshipByArg2(arg2, star);
	}
	default:
		return callStorage.getAllRelationship(star);
	}
}
