#include "./GetFollowsRequest.h"

GetFollowsRequest::GetFollowsRequest(Arg arg1, Arg arg2, Mode mode, HasStar star) :
	GetRelationshipRequest(arg1, arg2, mode), hasStar(star) {}

GetFollowsRequest::GetFollowsRequest(Arg arg, Mode mode, HasStar star) :
	GetRelationshipRequest(arg, mode), hasStar(star) {}

GetFollowsRequest::GetFollowsRequest(Mode mode, HasStar star) :
	GetRelationshipRequest(mode), hasStar(star) {}

HasStar GetFollowsRequest::getHasStar() const {
	return this->hasStar;
}

RelationshipResults GetFollowsRequest::execute(RelationshipStorage& storage) {
	Mode mode = getMode();
	//As long as we know got star, can just put star as parameters (checking if has star will be done internally)
	HasStar star = getHasStar();
	FollowsStorage followsStorage = storage.getFollowsStorage();
	switch (mode) {
	case Mode::FILTER_ARG1_ARG2:
	{
		Arg arg1 = getArg1();
		Arg arg2 = getArg2();
		return followsStorage.getRelationshipByArg1AndArg2(arg1, arg2, star);
	}
	case Mode::FILTER_ARG_1:
	{
		Arg arg1 = getArg1();
		return followsStorage.getRelationshipByArg1(arg1, star);
	}
	case Mode::FILTER_ARG_2:
	{
		Arg arg2 = getArg2();
		return followsStorage.getRelationshipByArg2(arg2, star);
	}
	default:
		return followsStorage.getAllRelationship(star);
	}
}