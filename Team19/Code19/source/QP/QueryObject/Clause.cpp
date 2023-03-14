#include "Clause.h"

bool Clause::operator==(const Clause& clause) const
{
	return tie(clauseType) == tie(clause.clauseType);
}

bool Clause::operator!=(const Clause& clause) const
{
	return tie(clauseType) != tie(clause.clauseType);
}

bool Clause::operator<(const Clause& clause) const
{
	return this->penalty < clause.penalty;
}

bool Clause::hasSyn() { return false; }

vector<Declaration> Clause::getSynonyms() { return {}; }

std::unordered_set<std::string> Clause::getSynNames() { return {}; }

bool Clause::evaluateClauseWithNoSyn(GetFacade* pkb) { return false; }

bool Clause::evaluateClauseWithSynToBool(GetFacade* pkb) { return false; }

Table Clause::evaluateClauseWithSyn(GetFacade* pkb) { return Table(); }

std::string Clause::printClause() { return ""; };