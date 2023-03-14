#include "./GetNextRequest.h"

GetNextRequest::GetNextRequest(Arg arg1, Arg arg2, Mode mode, HasStar star) :
	GetRelationshipRequest(arg1, arg2, mode), hasStar(star) {}

GetNextRequest::GetNextRequest(Arg arg, Mode mode, HasStar star) :
	GetRelationshipRequest(arg, mode), hasStar(star) {}


GetNextRequest::GetNextRequest(Mode mode, HasStar star) :
	GetRelationshipRequest(mode), hasStar(star) {}

HasStar GetNextRequest::getHasStar() const {
	return this->hasStar;
}

RelationshipResults GetNextRequest::execute(RelationshipStorage& storage) {
	Mode mode = getMode();
	//As long as we know got star, can just put star as parameters (checking if has star will be done internally)
	HasStar star = getHasStar();
	NextStorage nextStorage = storage.getNextStorage();
	switch (mode) {
	case Mode::FILTER_ARG1_ARG2:
	{
		Arg arg1 = getArg1();
		Arg arg2 = getArg2();
		return nextStorage.getRelationshipByArg1AndArg2(storage.getCFGStorage(), arg1, arg2, star);
	}
	case Mode::FILTER_ARG_1:
	{
		Arg arg1 = getArg1();
		return nextStorage.getRelationshipByArg1(storage.getCFGStorage(), arg1, star);
	}
	case Mode::FILTER_ARG_2:
	{
		Arg arg2 = getArg2();
		return nextStorage.getRelationshipByArg2(storage.getCFGStorage(), arg2, star);
	}
	default:
		return nextStorage.getAllRelationship(storage.getCFGStorage(), star);
	}
}