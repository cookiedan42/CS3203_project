#include "AffectsTSynSyn.h"

AffectsTSynSyn::AffectsTSynSyn(GetFacade& facade) : facade(facade) {}

RelationshipResults AffectsTSynSyn::evaluate() {
	std::vector<std::map<stmtNo_t, std::set<stmtNo_t>>> tables{};
	auto cfgs = AffectsUtil::getAllCFGs(facade);
	for (const auto& cfg : cfgs) {
		RelationshipResults results = AffectsSynSyn(facade, *cfg).evaluate();

		// Convert results to table
		std::map<stmtNo_t, std::set<stmtNo_t>> table;
		for (const auto& [affecter, affected] : results) {
			if (table.count(stoi(affecter))) {
				table.at(stoi(affecter)).insert(stoi(affected));
			}
			else {
				table.insert({ stoi(affecter), { stoi(affected) } });
			}
		}
		tables.push_back(table);
	}

	// for each table --> BFS until all pairs solved
	RelationshipResults results{};

	for (auto& table : tables) {
		auto k = allPairs(table);
		results.insert(k.cbegin(), k.cend());
	}
	
	return results;
}

RelationshipResults AffectsTSynSyn::allPairs(std::map<stmtNo_t, std::set<stmtNo_t>> table) {
	// for each entry, BFS --> update values
	for (auto& [k, v] : table) {
		v = allDest(k, table);
	}

	RelationshipResults results{};
	for (auto& [affecter, affecteds] : table) {
		auto affecter_str = std::to_string(affecter);
		for (auto& affected : affecteds) {
			results.insert({ affecter_str,std::to_string(affected) });
		}
	}
	return results;
}

std::set<stmtNo_t> AffectsTSynSyn::allDest(stmtNo_t start, std::map<stmtNo_t, std::set<stmtNo_t>> table) {
	std::set<stmtNo_t> visited{};
	std::set<stmtNo_t> results{};

	std::deque<stmtNo_t> stmtQueue{ table.at(start).cbegin(), table.at(start).cend() };
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

// all pairs paths

// from affects sub algo that solves by cfg
/*
STMT: AFFECTED_BY
1
2
3
4
loop each until no change
loop all until all no change
sorted set --> subset efficient
*/

//for each value, append to current list until no change?
