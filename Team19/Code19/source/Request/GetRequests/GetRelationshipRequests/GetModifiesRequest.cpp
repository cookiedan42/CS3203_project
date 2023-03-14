#include "./GetModifiesRequest.h"

GetModifiesRequest::GetModifiesRequest(Arg arg1, Arg arg2, Mode mode, Arg1Type arg1Type) :
	GetRelationshipRequest(arg1, arg2, mode), arg1Type(arg1Type) {
	this->hasArg1Type = arg1Type != Arg1Type::ANY;
}

GetModifiesRequest::GetModifiesRequest(Arg arg, Mode mode, Arg1Type arg1Type) :
	GetRelationshipRequest(arg, mode), arg1Type(arg1Type) {
	this->hasArg1Type = arg1Type != Arg1Type::ANY;
}


GetModifiesRequest::GetModifiesRequest(Mode mode, Arg1Type arg1Type) :
	GetRelationshipRequest(mode), arg1Type(arg1Type) {
	this->hasArg1Type = arg1Type != Arg1Type::ANY;
}

bool GetModifiesRequest::getHasArg1Type() const {
	return hasArg1Type;
}

Arg1Type GetModifiesRequest::getArg1Type() const {
	return arg1Type;
}

RelationshipResults GetModifiesRequest::execute(RelationshipStorage& storage) {
	Mode mode = getMode();
	Arg1Type arg1Type = getArg1Type();
	ModifiesStorage modifiesStorage = storage.getModifiesStorage();

	switch (mode) {
	case Mode::FILTER_ARG1_ARG2:
	{
		Arg arg1 = getArg1();
		Arg arg2 = getArg2();
		return modifiesStorage.getRelationshipByArg1AndArg2(arg1, arg2, arg1Type);
	}
	case Mode::FILTER_ARG_1:
	{
		Arg arg1 = getArg1();
		return  modifiesStorage.getRelationshipByArg1(arg1, arg1Type);
	}
	case Mode::FILTER_ARG_2:
	{
		Arg arg2 = getArg2();
		return modifiesStorage.getRelationshipByArg2(arg2, arg1Type);
	}
	default:
	{
		return getHasArg1Type() ? modifiesStorage.getAllRelationship(arg1Type)
			: modifiesStorage.getAllRelationship();
	}
	}
}