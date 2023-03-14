#include "CallValidator.h"

void CallValidator::setProc(std::string newProc) {
	this->currentProc = newProc;
	// new proc should not be in all_procs
	if (std::find(allProcs.cbegin(), allProcs.cend(), newProc) != allProcs.cend()) {
		throw validation_error(Exception::SEMANTIC_ERROR);
	}
	allProcs.push_back(newProc);
	callsGraph.insert({ newProc ,{} });
}

void CallValidator::addCall(std::string newCall) {
// current proc is calling newCall
	callsGraph[currentProc].push_back(newCall);
	allCalled.insert(newCall);
}

bool CallValidator::isValid() {
	// order ensures that no_invalid is tested first
	// recurse/loop will break if invalid calls exist 
	if (noInvalid() && noRecurse() && noLoop()) {
		return true;
	}
	return false;
}

bool CallValidator::noRecurse() {
	for each (auto proc in callsGraph) {
		auto procName = proc.first;
		auto calledProc = proc.second;
		// if key found in vals, it's recursive
		if (std::find(calledProc.cbegin(),calledProc.cend(), procName) != calledProc.cend()) {
			return false;
		}
	}
	return true;
}

bool CallValidator::noInvalid() {
// all_calls must be subset of all_procs
	if (allCalled.size() > allProcs.size()) {
		return false;
	}

	// every called proc should be in all procs
	for each (auto called in allCalled) {
		if (std::find(allProcs.cbegin(), allProcs.cend(), called) == allProcs.cend()) {
			return false;
		}
	}
	return true;
}

bool CallValidator::noLoop() {
	// Use DFS
	std::unordered_map <std::string, bool> visited {};
	for each (auto proc_name in allProcs) {
		visited.insert({ proc_name,false });
	}

	// using vector as searchable stack
	std::vector<std::string> path {};

	for each (auto proc in callsGraph) {
		auto key = proc.first;
		auto has_no_loop = noLoopRec(path,visited,key);
		if (!has_no_loop) { return false;}
	}
	return true;
}


/*
	return false if loop found
*/
bool CallValidator::noLoopRec(
	std::vector<std::string>& path,
	std::unordered_map<std::string,bool>& visited,
	std::string currentNode
) {

	// if this node is on path, loop found
	if (std::find(path.cbegin(), path.cend(), currentNode) != path.cend()) {
		return false;
	}

	// if this node has been visited before, skip
	if (visited.find(currentNode)->second) {
		return true;
	}

	path.push_back(currentNode);
	visited.find(currentNode)->second = true;

	auto local_proc = callsGraph.find(currentNode)->second;
	for each (auto called in local_proc) {
		auto child_has_no_loop = noLoopRec(path, visited, called);
		if (!child_has_no_loop) { return false; }
	}

	// all children of this path had no loops
	path.pop_back();
	return true;
}

