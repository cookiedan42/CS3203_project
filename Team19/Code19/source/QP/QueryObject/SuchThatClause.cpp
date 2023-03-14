#include "SuchThatClause.h"
#include "../QueryEvaluator/SuchThatEvaluator.h"

SuchThatClause::SuchThatClause(shared_ptr<DesignAbstraction> relationship)
	: relationship(relationship)
{
	this->clauseType = "SuchThatClause";
	this->penalty = SUCH_THAT_CLAUSE_PENALTY + relationship->relationshipPenalty;

	//stores the synonym and synNames present in the Clause.
	ArgType arg1Type = QueryUtil::getArgTypeFromArgument(this->relationship->arg1);
	ArgType arg2Type = QueryUtil::getArgTypeFromArgument(this->relationship->arg2);
	if (arg1Type == ArgType::SYN) {
		Declaration arg1Dec = std::get<Declaration>((this->relationship->arg1).argumentVariant);
		this->synonyms.push_back(arg1Dec);
		this->synNames.emplace(arg1Dec.synName);
		this->penalty += SYN_PENALTY;
	}
	if (arg2Type == ArgType::SYN) {
		Declaration arg2Dec = std::get<Declaration>((this->relationship->arg2).argumentVariant);
		this->synonyms.push_back(arg2Dec);
		this->synNames.emplace(arg2Dec.synName);
		this->penalty += SYN_PENALTY;
	}
}

bool SuchThatClause::operator==(const SuchThatClause& other) const 
{
	if (this == &other) {
		return true;
	}
	return (relationship->arg1 == relationship->arg1) &&
		(relationship->arg2 == relationship->arg2) &&
		(relationship->type == relationship->type);
}

bool SuchThatClause::hasSyn()
{
	return this->synonyms.size() > 0;
}

vector<Declaration> SuchThatClause::getSynonyms()
{
	return this->synonyms;
}

std::unordered_set<std::string> SuchThatClause::getSynNames() {
	return this->synNames;
}

bool SuchThatClause::evaluateClauseWithNoSyn(GetFacade* pkb)
{
	return SuchThatEvaluator::evaluateClauseWithNoSyn(make_shared<SuchThatClause>(this->relationship), pkb);
}

bool SuchThatClause::evaluateClauseWithSynToBool(GetFacade* pkb)
{
	return SuchThatEvaluator::evaluateClauseWithSynToBool(make_shared<SuchThatClause>(this->relationship), pkb);
}

Table SuchThatClause::evaluateClauseWithSyn(GetFacade* pkb)
{
	return SuchThatEvaluator::evaluateClauseWithSyn(make_shared<SuchThatClause>(this->relationship), pkb);
}

std::string SuchThatClause::printClause() {
	string toReturn = this->relationship->type + ": ";
	for (auto& sn : this->synNames) {
		toReturn += sn + ",";
	}
	return toReturn + " with penalty: " + std::to_string(this->penalty);
}