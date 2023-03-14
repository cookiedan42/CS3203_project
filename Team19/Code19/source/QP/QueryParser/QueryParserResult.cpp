#include "./QueryParserResult.h"

QueryParserResult::QueryParserResult(std::vector<Declaration> declarations, SelectObject selectObj,
	std::vector<SuchThatClause> noSynSTClauses, std::vector<SuchThatClause> hasSynSTClauses,
	std::vector<PatternClause> noSynPTClauses, std::vector<PatternClause> hasSynPTClauses,
	std::vector<WithClause> noSynWithClauses, std::vector<WithClause> hasSynWithClauses)

{
	this->declarations = declarations;
	this->selectObj = selectObj;

	QueryParserResult::processSuchThatClauses(noSynSTClauses, hasSynSTClauses);
	QueryParserResult::processPatternClauses(noSynPTClauses, hasSynPTClauses);
	QueryParserResult::processWithClauses(noSynWithClauses, hasSynWithClauses);
}

std::vector<Declaration> QueryParserResult::getDeclarations() const {
	return this->declarations;
}

SelectObject QueryParserResult::getSelectObj() const {
	return this->selectObj;
}

ClauseGroup QueryParserResult::getNoSynClauses() const {
	return this->noSynClauses;
}

std::deque<ClauseGroup> QueryParserResult::getClausesWithAtLeastOneSynonym() const {
	return this->clausesWithAtLeastOneSynonym;
}

bool QueryParserResult::operator==(const QueryParserResult& qo) const {
	return tie(declarations, selectObj, noSynClauses, clausesWithAtLeastOneSynonym)
		== tie(qo.declarations, qo.selectObj, qo.noSynClauses, qo.clausesWithAtLeastOneSynonym);
}

void QueryParserResult::processSuchThatClauses(std::vector<SuchThatClause> &noSynClauses, std::vector<SuchThatClause>& hasSynClauses) {
	for (auto& cl : noSynClauses) {
		this->noSynClauses.addClauseToGroup(make_shared<SuchThatClause>(cl.relationship));
	}

	for (auto& cl : hasSynClauses) {
		ClauseGroup cg;
		cg.addClauseToGroup(make_shared<SuchThatClause>(cl.relationship));
		this->clausesWithAtLeastOneSynonym.push_back(cg);
	}
}

void QueryParserResult::processPatternClauses(std::vector<PatternClause>& noSynClauses, std::vector<PatternClause>& hasSynClauses) {
	for (auto& cl : noSynClauses) {
		this->noSynClauses.addClauseToGroup(make_shared<PatternClause>(cl.synonym, cl.arg_1, cl.arg_2));
	}

	for (auto& cl : hasSynClauses) {
		ClauseGroup cg;
		cg.addClauseToGroup(make_shared<PatternClause>(cl.synonym, cl.arg_1, cl.arg_2));
		this->clausesWithAtLeastOneSynonym.push_back(cg);
	}
}

void QueryParserResult::processWithClauses(std::vector<WithClause>& noSynClauses, std::vector<WithClause>& hasSynClauses) {
	for (auto& cl : noSynClauses) {
		this->noSynClauses.addClauseToGroup(make_shared<WithClause>(cl.arg1, cl.arg2));
	}

	for (auto& cl : hasSynClauses) {
		ClauseGroup cg;
		cg.addClauseToGroup(make_shared<WithClause>(cl.arg1, cl.arg2));
		this->clausesWithAtLeastOneSynonym.push_back(cg);
	}
}