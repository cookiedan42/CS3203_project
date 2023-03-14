#include "Integer.h"


Integer::Integer(std::string value)
{
	this->value = value;
}

bool Integer::operator==(const Integer& integer) const {
	return tie(value) == tie(integer.value);
}

bool Integer::operator!=(const Integer& integer) const {
	return (tie(value) != tie(integer.value));
}
