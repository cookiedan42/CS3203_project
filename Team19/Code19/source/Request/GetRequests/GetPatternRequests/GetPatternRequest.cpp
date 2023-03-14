#include "./GetPatternRequest.h"

GetPatternRequest::GetPatternRequest(PatternType type, PatternMode mode, Arg arg1, Arg arg2) {
	this->type = type;
	this->mode = mode;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->hasArg1 = arg1 != EMPTY_ARG;
	this->hasArg2 = arg2 != EMPTY_ARG;
}

PatternType GetPatternRequest::getType() const {
	return type;
}

PatternMode GetPatternRequest::getMode() const {
	return mode;
}
Arg GetPatternRequest::getArg1() const {
	return arg1;
}

Arg GetPatternRequest::getArg2() const {
	return arg2;
}

bool GetPatternRequest::getHasArg1() const {
	return hasArg1;
}

bool GetPatternRequest::getHasArg2() const {
	return hasArg1;
}

