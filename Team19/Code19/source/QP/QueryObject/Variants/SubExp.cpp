#include "SubExp.h"


SubExp::SubExp(std::string value)
{
	this->value = value;
}

bool SubExp::operator==(const SubExp& subExp) const {
	return tie(value) == tie(subExp.value);
}

bool SubExp::operator!=(const SubExp& subExp) const {
	return (tie(value) != tie(subExp.value));
}
