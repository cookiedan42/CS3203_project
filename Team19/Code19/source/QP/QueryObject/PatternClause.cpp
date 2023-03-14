#include "PatternClause.h"
#include "../QueryEvaluator/PatternEvaluator.h"

PatternClause::PatternClause(Declaration syn, Argument arg_1, Argument arg_2)
	: synonym(syn), arg_1(arg_1), arg_2(arg_2)
{
	this->clauseType = "PatternClause";

	//stores the synonym and synNames present in the Clause.
	this->synonyms.push_back(this->synonym);
	this->synNames.emplace(this->synonym.synName);
	this->penalty = PATTERN_CLAUSE_PENALTY + SYN_PENALTY;

	ArgType arg1Type = QueryUtil::getArgTypeFromArgument(this->arg_1);
	if (arg1Type == ArgType::SYN) {
		Declaration arg1Dec = std::get<Declaration>((this->arg_1).argumentVariant);
		this->synonyms.push_back(arg1Dec);
		this->synNames.emplace(arg1Dec.synName);
		this->penalty += SYN_PENALTY;
	}
}

bool PatternClause::operator==(const PatternClause& other) const {
 if (this == &other) {
  return true;
 }
 return (this->synonym == other.synonym) && tie(this->arg_1, this->arg_2) == tie(other.arg_1, other.arg_2);
}

bool PatternClause::hasSyn() 
{
	return this->synonyms.size() > 0;
}

vector<Declaration> PatternClause::getSynonyms() {
	return this->synonyms;
}

std::unordered_set<std::string> PatternClause::getSynNames() {
	return this->synNames;
}

bool PatternClause::evaluateClauseWithNoSyn(GetFacade* pkb)
{
	return PatternEvaluator::evaluateNoCommSyn(make_shared<PatternClause>(this->synonym, this->arg_1, this->arg_2), pkb);
}

bool PatternClause::evaluateClauseWithSynToBool(GetFacade* pkb)
{
	return PatternEvaluator::evaluateNoCommSyn(make_shared<PatternClause>(this->synonym, this->arg_1, this->arg_2), pkb);
}

Table PatternClause::evaluateClauseWithSyn(GetFacade* pkb)
{
	return PatternEvaluator::evaluateHasCommSyn(make_shared<PatternClause>(this->synonym, this->arg_1, this->arg_2), pkb);
}

std::string PatternClause::printClause() {
	string toReturn = this->clauseType + ": ";
	for (auto& sn : this->synNames) {
		toReturn += sn + ",";
	}
	return toReturn + " with penalty: " + std::to_string(this->penalty);
}


bool PatternClause::isIfPattern() const { 
	return synonym.synType->type == IF_DE; 
}
bool PatternClause::isWhilePattern() const { 
	return synonym.synType->type == WHILE_DE; 
}
bool PatternClause::isAssignPattern() const { 
	return synonym.synType->type == ASSIGN_DE; 
}

