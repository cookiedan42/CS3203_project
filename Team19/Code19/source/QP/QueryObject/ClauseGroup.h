#pragma once

#include "./Clause.h"
#include "./SuchThatClause.h"

class ClauseGroup {
public:
	ClauseGroup::ClauseGroup();
	void addClauseToGroup(shared_ptr<Clause> clause);
	bool hasCommSyn(std::unordered_set<std::string> syn);
	void merge(ClauseGroup group);
	void sort();
	bool operator==(const ClauseGroup& other) const;

	std::vector<shared_ptr<Clause>> getRelatedClauses() const;
	std::unordered_set<std::string> getGroupSynNames() const;

private:
	std::vector<shared_ptr<Clause>> relatedClauses;
	std::unordered_set<std::string> groupSynNames;
};
