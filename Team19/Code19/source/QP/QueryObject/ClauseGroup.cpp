#include "./ClauseGroup.h"

ClauseGroup::ClauseGroup() {}

std::vector<shared_ptr<Clause>> ClauseGroup::getRelatedClauses() const {
	return this->relatedClauses;
}

std::unordered_set<std::string> ClauseGroup::getGroupSynNames() const {
	return this->groupSynNames;
}

//Adds a clause to the current group
//Updates the synonym set as well.
void ClauseGroup::addClauseToGroup(shared_ptr<Clause> clause) {
	relatedClauses.emplace_back(clause);
	std::unordered_set<std::string> names = clause->getSynNames();
	this->groupSynNames.insert(names.begin(), names.end());
}

//O(n^2) loop is fine because #synNames are small.
bool ClauseGroup::hasCommSyn(std::unordered_set<std::string> syn) {
	for (auto& name : this->groupSynNames) {
		for (auto& otherName : syn) {
			if (name == otherName) {
				return true;
			}
		}
	}
	return false;
}

void ClauseGroup::merge(ClauseGroup group) {
	auto otherClauses = group.getRelatedClauses();
	auto names = group.getGroupSynNames();
	this->relatedClauses.insert(this->relatedClauses.end(), otherClauses.begin(), otherClauses.end());
	this->groupSynNames.insert(names.begin(), names.end());
}

bool compareClausePointer(shared_ptr<Clause> a, shared_ptr<Clause> b) {
	return (*a < *b);
}

void ClauseGroup::sort() {
	std::sort(relatedClauses.begin(), relatedClauses.end(), compareClausePointer);
}

bool ClauseGroup::operator==(const ClauseGroup& other) const {
	if (this == &other) {
		return true;
	}

	return equal(begin(relatedClauses), end(relatedClauses),
		begin(other.relatedClauses), end(other.relatedClauses),
		[](const shared_ptr<Clause> lhs, const shared_ptr<Clause> rhs) { return *lhs == *rhs; });


}