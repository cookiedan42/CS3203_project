#include "StringExp.h"


StringExp::StringExp(std::string value)
{
	this->value = value;
}

bool StringExp::operator==(const StringExp& stringExp) const {
	return tie(value) == tie(stringExp.value);
}

bool StringExp::operator!=(const StringExp& stringExp) const {
	return (tie(value) != tie(stringExp.value));
}
