#include "WithClause.h"
#include "../QueryEvaluator/WithEvaluator.h"


WithClause::WithClause()
{
	this->argsType = ArgsType::EMPTY;
}

bool WithClause::operator==(const WithClause& other) const {
	if (this == &other) {
		return true;
	}
	return tie(this->arg1, this->arg2) == tie(other.arg1, other.arg2);
}

WithClause::WithClause(Argument arg1, Argument arg2)
{
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->clauseType = "WithClause";
	this->penalty = WITH_CLAUSE_PENALTY;

	//stores the synonym and synNames present in the Clause.
	ArgType arg1Type = QueryUtil::getArgTypeFromArgument(this->arg1);
	ArgType arg2Type = QueryUtil::getArgTypeFromArgument(this->arg2);
	if (arg1Type == ArgType::ATTR_REF_INT || arg1Type == ArgType::ATTR_REF_NAME) {
		Declaration syn1 = std::get<SynAttr>(arg1.argumentVariant).synonym;
		this->synonyms.push_back(syn1);
		this->synNames.emplace(syn1.synName);
		this->penalty += SYN_PENALTY;

	}
	if (arg2Type == ArgType::ATTR_REF_INT || arg2Type == ArgType::ATTR_REF_NAME) {
		Declaration syn2 = std::get<SynAttr>(arg2.argumentVariant).synonym;
		this->synonyms.push_back(syn2);
		this->synNames.emplace(syn2.synName);
		this->penalty += SYN_PENALTY;
	}

}

bool WithClause::hasSyn()
{
	return this->synonyms.size() > 0;
}

vector<Declaration> WithClause::getSynonyms()
{
	return this->synonyms;
}

std::unordered_set<std::string> WithClause::getSynNames() {
	return this->synNames;
}

bool WithClause::evaluateClauseWithNoSyn(GetFacade* pkb)
{
	return WithEvaluator::evaluateNoCommSyn(make_shared<WithClause>(this->arg1, this->arg2), pkb);
}

bool WithClause::evaluateClauseWithSynToBool(GetFacade* pkb)
{
	return WithEvaluator::evaluateNoCommSyn(make_shared<WithClause>(this->arg1, this->arg2), pkb);
}

Table WithClause::evaluateClauseWithSyn(GetFacade* pkb)
{
	return WithEvaluator::evaluateHasCommSyn(make_shared<WithClause>(this->arg1, this->arg2), pkb);
}

std::string WithClause::printClause() {
	string toReturn = this->clauseType + ": ";
	for (auto& sn : this->synNames) {
		toReturn += sn + ",";
	}
	return toReturn + " with penalty: " + std::to_string(this->penalty);
}