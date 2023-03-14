#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include "../../Definitions/Definitions.h"
#include "../../TypeAlias/TypeAlias.h"

using validation_error = std::invalid_argument;

class CallValidator {
	// table with parent child
	// assuming that no-nested invariant has been established
	// all calls between procedures belong to the prior procedure
	std::vector<std::string> allProcs = {};
	CalledSet allCalled = {};
	std::string currentProc = "";
	CallsGraph callsGraph = {};

	bool noLoopRec(std::vector<std::string>& path,
		std::unordered_map<std::string, bool>& visited, std::string currentNode);

public:
	CallValidator() {};
	
	void setProc(std::string newProc);
	void addCall(std::string newCall);
	bool isValid();

	bool noRecurse();
	bool noLoop();
	bool noInvalid();
};