#include "./Cfg.h"

void Cfg::insert(const stmtNo_t& front, const stmtNo_t& back) {
	if (back > lastStmtNo) { lastStmtNo = back; }
	if (forwardCFG.find(front) == forwardCFG.cend()) {
		forwardCFG.insert({ front,{} });
	}
	if (backwardCFG.find(back) == backwardCFG.cend()) {
		backwardCFG.insert({ back, {} });
	}
	forwardCFG.at(front).insert({ back });
	backwardCFG.at(back).insert({ front });
}
void Cfg::insert(const stmtNo_t& stmtNo, const std::string stmtType) {
	stmtTypeMap.insert({ stmtNo,stmtType });
}

bool Cfg::contains(const stmtNo_t& target) const { 
	return target >= firstStmtNo && target <= lastStmtNo; 
}

stmtNoSet_t Cfg::getNextof(const stmtNo_t& target) const {
	if (forwardCFG.find(target) == forwardCFG.cend()) { return {}; }
	return forwardCFG.at(target);
}

stmtNoSet_t Cfg::getPrevof(const stmtNo_t& target) const {
	if (backwardCFG.find(target) == backwardCFG.cend()) { return {}; }
	return backwardCFG.at(target);
}


bool Cfg::hasPath(const stmtNo_t& start, const stmtNo_t& end) const {
	if (!contains(start)) { return false; }
	if (!contains(end)) { return false; }
	return hasPath_node(start, end);
}

bool Cfg::hasPath_node(const stmtNo_t& start, const stmtNo_t& end) const {
	// use BFS to traverse
	stmtNoSet_t seen {};
	auto vqdata = getNextof(start);
	std::deque<stmtNo_t> visitQueue { vqdata.cbegin(),vqdata.cend() };

	while (!visitQueue.empty()) {
		const stmtNo_t current = visitQueue.front();
		visitQueue.pop_front();
		if (current == end) { return true; }
		// if seen before, skip 
		if (seen.find(current) != seen.cend()) { continue; }
		seen.insert(current);
		// if no followers, skip
		if (getNextof(current).empty()) { continue; }
		const stmtNoSet_t& followers = getNextof(current);
		visitQueue.insert(visitQueue.cend(), followers.cbegin(), followers.cend());
	}
	return false;
}

// Next*(int,syn)
stmtNoSet_t Cfg::getNextStar(const stmtNo_t& start) const {
	auto fillFunc = [this](const stmtNo_t& i) {return getNextof(i); };
	auto fullList = getXStar(start, fillFunc);
	stmtNoSet_t out {};
	for each (auto  entry in fullList) {
		if (entry <= 0) { continue; }
		out.insert(entry);
	}
	return out;
}
// Next*(syn,int)
stmtNoSet_t Cfg::getPrevStar(const stmtNo_t& end) const {
	auto fillFunc = [this](const stmtNo_t& i) {return getPrevof(i); };
	auto fullList = getXStar(end, fillFunc);
	stmtNoSet_t out {};
	for each (auto  entry in fullList) {
		if (entry <= 0) { continue; }
		out.insert(entry);
	}
	return out;
}


// common algo for Next*(syn,int) and Next*(int,syn)
stmtNoSet_t Cfg::getXStar(const stmtNo_t& startPt, std::function<stmtNoSet_t (const stmtNo_t&)> filler) const {
	if (!contains(startPt)) { return {}; }

	stmtNoSet_t seen {};
	stmtNoSet_t a = filler(startPt);
	std::deque<stmtNo_t> visitQueue { a.cbegin(),a.cend() };

	while (!visitQueue.empty()) {
		const stmtNo_t current = visitQueue.front();
		visitQueue.pop_front();
		// if seen before, skip 
		if (seen.find(current) != seen.cend()) { continue; }
		seen.insert(current);
		if (filler(current).empty()) { continue; }
		const stmtNoSet_t& followers = filler(current);
		visitQueue.insert(visitQueue.cend(), followers.cbegin(), followers.cend());
	}
	return seen;
}


bool Cfg::hasNextStar(const stmtNo_t& start) const {
	for each (auto v in getNextof(start)) {
		// if positive exists, good
		// if negative exists, check if it has a next
		//	if negative has a next, good
		//	if negative has no next, try next value
		if (v > 0) { return true; }
		if (hasNextStar(v)) { return true; }
	}
	return false;
}
bool Cfg::hasPrevStar(const stmtNo_t& end) const {
	for each (auto v in getPrevof(end)) {
		// if positive exists, good
		// if negative exists, check if it has a next
		//	if negative has a next, good
		//	if negative has no next, try next value
		if (v > 0) { return true; }
		if (hasPrevStar(v)) { return true; }
	}
	return false;
}

bool Cfg::hasNextStarWildWild() const {
	return hasNextStar(firstStmtNo);
}

stmtNoSet_t Cfg::getNextStarSynWild() const {
	// everything except last statements
	
	stmtNoSet_t result {};
	for (stmtNo_t i = firstStmtNo; i <=lastStmtNo; i++) {
		result.insert(i);
	}


	//backward traversal
	std::deque<stmtNo_t> toCheck {};

	for (auto& var : getPrevof(0)) {
		toCheck.push_back(var);
	} ;

	while (!toCheck.empty()) {
		auto val = toCheck.front();
		toCheck.pop_front();

		if (val < 0) {
			for (auto& var : getPrevof(val)) {
				toCheck.push_back(var);
			};
			continue;
		}

		for (auto& var : getNextof(val)) {
			if (var > 0) { continue; }
		};
		result.erase(val);
	}
	return result;
};

stmtNoSet_t Cfg::getNextStarWildSyn() const {
	// everything except first statement
	stmtNoSet_t result {};
	for (stmtNo_t i = firstStmtNo+1; i <= lastStmtNo; i++) {
		result.insert(i);
	}
	return result;
};

std::vector<std::pair<stmtNo_t, stmtNo_t>> Cfg::getNextStarSynSyn() const {
	if (!hasNextStarWildWild()) { return {}; }

	// slow way
	std::vector<std::pair<stmtNo_t, stmtNo_t>> result {};

	std::map<stmtNo_t, std::set<stmtNo_t>> nexts {};

	//get all nexts
	for (auto& [key, vals] : forwardCFG) {
		if (key <= 0) { continue; }
		nexts.insert({ key,{} });
		for each (auto n in getPosNextRec(key)) {
			nexts.at(key).insert(n);
		}
	}

	for (auto& [k, v] : nexts) {
		v = Cfg::allDest(k, nexts);
	}


	for (auto& [key, values] : nexts) {
		for (auto& value : values) {
			result.push_back(std::pair<stmtNo_t, stmtNo_t>{key, value});

		}
	}

	return result;
}

std::set<stmtNo_t> Cfg::allDest(stmtNo_t start, std::map<stmtNo_t, std::set<stmtNo_t>> table) const{
	std::set<stmtNo_t> visited {};
	std::set<stmtNo_t> results {};

	std::deque<stmtNo_t> stmtQueue { table.at(start).cbegin(), table.at(start).cend() };
	while (!stmtQueue.empty()) {
		auto current = stmtQueue.front();
		stmtQueue.pop_front();

		if (visited.find(current) != visited.cend()) continue;
		visited.insert(current);
		results.insert(current);

		// insert next nodes into queue if exist
		if (table.find(current) == table.cend()) continue;
		stmtQueue.insert(stmtQueue.cend(), table.at(current).cbegin(), table.at(current).cend());
	}
	return results;
}

std::vector<stmtNo_t> Cfg::getPosNextRec(stmtNo_t val) const {
	std::vector<stmtNo_t> res {};
	
	for each (auto& n in getNextof(val)) {
		if (n > 0) {
			res.push_back(n);
		}
		else {
			auto ns = getPosNextRec(n);
			res.insert(res.cend(), ns.cbegin(), ns.cend());
		}
	}
	return res;
}


std::vector<std::pair<std::string, std::string>> Cfg::getAllNext() const {
	std::vector<std::pair<std::string, std::string>> res{};
	for (auto& [key, vals] : forwardCFG) {
		if (key <= 0) { continue; }
		auto keyStr = std::to_string(key);
		for each (auto n in getPosNextRec(key)) {
			res.push_back({ keyStr, std::to_string(n) });
		}
	}
	return res;
}

std::string Cfg::getStmtType(stmtNo_t target) const {
	if (!contains(target)) { return ""; }
	return stmtTypeMap.at(target);
}
std::vector <stmtNo_t> Cfg::getAssignStmts() const {
	std::vector <stmtNo_t> result {};
	for (auto& [stmtNo,stmtType] : stmtTypeMap) {
		if (stmtType != Ent::assign_stmtType) { continue; }
		result.push_back(stmtNo);
	}
	return result;
}
