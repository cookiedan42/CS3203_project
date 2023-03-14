#include "./GetUsesRequest.h"

GetUsesRequest::GetUsesRequest(Arg arg1, Arg arg2, Mode mode, Arg1Type arg1Type) :
	GetRelationshipRequest(arg1, arg2, mode), arg1Type(arg1Type) {
	this->hasArg1Type = arg1Type != Arg1Type::ANY;
}

GetUsesRequest::GetUsesRequest(Arg arg, Mode mode, Arg1Type arg1Type) :
	GetRelationshipRequest(arg, mode), arg1Type(arg1Type) {
	this->hasArg1Type = arg1Type != Arg1Type::ANY;
}


GetUsesRequest::GetUsesRequest(Mode mode, Arg1Type arg1Type) :
	GetRelationshipRequest(mode), arg1Type(arg1Type) {
	this->hasArg1Type = arg1Type != Arg1Type::ANY;
}

bool GetUsesRequest::getHasArg1Type() const {
	return this->hasArg1Type;
}

Arg1Type GetUsesRequest::getArg1Type() const {
	return this->arg1Type;
}

RelationshipResults GetUsesRequest::execute(RelationshipStorage& storage) {
	Mode mode = getMode();
	Arg1Type arg1Type = getArg1Type();
	UsesStorage usesStorage = storage.getUsesStorage();
	switch (mode) {
	case Mode::FILTER_ARG1_ARG2:
	{
		Arg arg1 = getArg1();
		Arg arg2 = getArg2();
		return usesStorage.getRelationshipByArg1AndArg2(arg1, arg2, arg1Type);
	}
	case Mode::FILTER_ARG_1:
	{
		Arg arg1 = getArg1();
		return  usesStorage.getRelationshipByArg1(arg1, arg1Type);
	}
	case Mode::FILTER_ARG_2:
	{
		Arg arg2 = getArg2();
		return usesStorage.getRelationshipByArg2(arg2, arg1Type);
	}
	default:
	{
		return getHasArg1Type() ? usesStorage.getAllRelationship(arg1Type)
			: usesStorage.getAllRelationship();
	}
	}
}