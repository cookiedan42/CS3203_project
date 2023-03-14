#include "SelectObject.h"

SelectObject::SelectObject()
{
	this->type = SelectType::BOOLEAN;
}

SelectObject::SelectObject(std::vector<selectPair> selectPairs): selectPairs(selectPairs) 
{
	if (selectPairs.size() == 1) {
		this->type = SelectType::SINGLE;
	}
	else if (selectPairs.size() > 1) {
		this->type = SelectType::TUPLE;
	}
	else {
		throw invalid_argument("Expected at least one declaration in SelectObject.");
	}
}

bool SelectObject::operator==(const SelectObject& so) const {
	return (distance(selectPairs.cbegin(), selectPairs.cend()) == distance(so.selectPairs.cbegin(), so.selectPairs.cend())) &&
		equal(selectPairs.cbegin(), selectPairs.cend(), so.selectPairs.cbegin());
}
