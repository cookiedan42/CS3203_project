#pragma once
#include <vector>
#include <variant>
#include "../QueryObject/Declaration.h"
#include "../../Definitions/Definitions.h"

struct AttrRef {
	Declaration synonym;
	std::string attrName;

	bool AttrRef::isNameType();
	bool AttrRef::isIntType();
};