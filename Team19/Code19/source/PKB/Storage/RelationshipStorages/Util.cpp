#include "./Util.h"

Util::Util() {}

void Util::insertIntoTable(std::string key, stringPair relationshipPair, intToStringPairMap& storage) {
	if (storage.count(key)) {
		//key found
		stringPairSet curr_set = storage[key];
		curr_set.insert(relationshipPair);
		storage[key] = curr_set;
	}
	else {
		stringPairSet new_set;
		new_set.insert(relationshipPair);
		storage[key] = new_set;
	}
}

void Util::globalInsert(stringPair relationship, stringPairSet& globalSet) {
	globalSet.insert(relationship);
}

RelationshipResults Util::handleGetRelationshipByArg1AndArg2(intToStringPairMap& map, std::string arg1, std::string arg2) {
	RelationshipResults toReturn;
	RelationshipResults byArg1 = map[arg1];
	stringPair relationshipPair(arg1, arg2);

	//assumes Relationship(arg1, arg2) is always unique
	if (byArg1.find(relationshipPair) != byArg1.end()) {
		toReturn.insert(relationshipPair);
	}
	return toReturn;
}

void Util::handleStar(intToStringPairMap& defaultMap, intToStringPairMap& starMap, intToStringPairMap& reverseMap, stringPairSet& globalSet) {
	for (const auto& [key, value] : defaultMap) {
		std::stack<std::string> stack;
		std::unordered_set<std::string> visited;
		RelationshipResults direct = defaultMap[key];

		visited.emplace(key);

		for (const auto& rs : direct) {
			stack.push(rs.second);
		}

		//WARNING: MIGHT INFINITE LOOP IF GOT CYCLIC TRANSITIVTY (1,4), (4,1) --> BUT THIS SHOULD NEVER HAPPEN
		while (!stack.empty()) {
			std::string newArg1 = stack.top();
			stack.pop();
			visited.emplace(newArg1);


			stringPair relationshipTo(key, newArg1);

			Util::insertIntoTable(key, relationshipTo, starMap);
			Util::insertIntoTable(newArg1, relationshipTo, reverseMap);
			Util::globalInsert(relationshipTo, globalSet);

			RelationshipResults indirect = defaultMap[newArg1];

			//only insert if not visited, else cyclic loop
			for (const auto& rs : indirect) {
				if (visited.find(rs.second) == visited.end()) {
					stack.push(rs.second);
				}
			}
		}
	}
}

RelationshipResults Util::getStar(CFGStorage& cfgStorage) {
	RelationshipResults results;

	std::vector<std::pair<stmtNo_t, stmtNo_t>> pairs;
	for (const auto& cfg : cfgStorage.getAllCFG()) {
		auto& intPairs = cfg->getNextStarSynSyn();
		for (const auto &p : intPairs) {
			stringPair rs(std::to_string(p.first), std::to_string(p.second));
			results.emplace(rs);
		}
	}
	return results;
}

RelationshipResults Util::getStarByArg1(CFGStorage& cfgStorage, Arg arg1) {
	RelationshipResults results;

	std::vector<std::pair<stmtNo_t, stmtNo_t>> pairs;

	const auto& cfgs = cfgStorage.getCFGByStmtNo(stoi(arg1));
	if (cfgs.empty()) { return results; }
	const shared_ptr<Cfg> cfg = *(cfgs.cbegin());


	auto arg2s = cfg->getNextStar(stoi(arg1));
	for (const auto& arg2 : arg2s) {
		stringPair rs(arg1, std::to_string(arg2));
		results.emplace(rs);
	}
	return results;
}

RelationshipResults Util::getStarByArg2(CFGStorage& cfgStorage, Arg arg2) {
	RelationshipResults results;

	std::vector<std::pair<stmtNo_t, stmtNo_t>> pairs;
	const auto& cfgs = cfgStorage.getCFGByStmtNo(stoi(arg2));
	if (cfgs.empty()) { return results; }
	const shared_ptr<Cfg> cfg = *(cfgs.cbegin());

	auto arg1s = cfg->getPrevStar(stoi(arg2));
	for (const auto& arg1 : arg1s) {
		stringPair rs(std::to_string(arg1), arg2);
		results.emplace(rs);
	}
	return results;
}

RelationshipResults Util::getStarByArg1AndArg2(CFGStorage& cfgStorage, Arg arg1, Arg arg2) {
	RelationshipResults results;
	std::vector<std::pair<stmtNo_t, stmtNo_t>> pairs;
	
	const auto& cfgs = cfgStorage.getCFGByStmtNo(stoi(arg1));
	if (cfgs.empty()) { return results; }
	const shared_ptr<Cfg> cfg = *(cfgs.cbegin());

	if (cfg->hasPath(stoi(arg1), stoi(arg2))) {
		stringPair rs(arg1, arg2);
		results.emplace(rs);
		return results;
	}
	return results;
}

void Util::handleGenerateNextData(CFGStorage& cfgStorage, intToStringPairMap& nextMap, intToStringPairMap& nextByMap, stringPairSet& globalNext) {
	for (const auto& cfg : cfgStorage.getAllCFG()) {
		std::vector<std::pair<std::string, std::string>> nextPairs = cfg->getAllNext();
		for (const auto& relationshipPair : nextPairs) {
			std::string s1 = relationshipPair.first;
			std::string s2 = relationshipPair.second;

			Util::insertIntoTable(s1, relationshipPair, nextMap);
			Util::insertIntoTable(s2, relationshipPair, nextByMap);
			Util::globalInsert(relationshipPair, globalNext);

		}
	}

}

