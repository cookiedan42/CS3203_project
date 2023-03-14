#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <stack>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <set>

#include "../../Entity/Entity.h"
#include "../../Relationship/Relationship.h"
#include "../../Pattern/Pattern.h"
#include "../../TypeAlias/TypeAlias.h"

using stmtNoKeyPatternValueMap = std::unordered_map<stmtNoRef, Ptn::AssignPtn>;

class Storage {
public:
	Storage() {}
};