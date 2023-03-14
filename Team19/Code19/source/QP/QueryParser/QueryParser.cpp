#include "QueryParser.h"

std::vector<Declaration> QueryParser::parseDeclarations(std::vector<QuerySplitter::dec_pair> declarations)
{
	return DeclarationsParser::parse(declarations);
}

SelectObject QueryParser::parseSelectObj(std::vector<Token> selectArg, std::vector<Declaration> declarations)
{
	return SelectParser::parse(selectArg, declarations);
}

SuchThatClauses QueryParser::parseSuchThatClauses(QuerySplitter::tokenized_clauses suchThatClauses, std::vector<Declaration> declarations)
{
	return SuchThatParser::parse(suchThatClauses, declarations);
}

PatternClauses QueryParser::parsePatternClauses(QuerySplitter::tokenized_clauses patternClauses, std::vector<Declaration> declarations)
{
	return PatternParser::parse(patternClauses, declarations);
}

WithClauses QueryParser::parseWithClauses(QuerySplitter::tokenized_clauses withClauses, std::vector<Declaration> declarations)
{
	return WithParser::parse(withClauses, declarations);
}

QueryParserResult QueryParser::parseQuery() {
	parsedDeclarations = parseDeclarations(declarations);
	parsedSelectObj = parseSelectObj(selectArg, parsedDeclarations);
	parsedSTCl = parseSuchThatClauses(suchThatClauses, parsedDeclarations);
	parsedPTCl = parsePatternClauses(patternClauses, parsedDeclarations);
	parsedWithCl = parseWithClauses(withClauses, parsedDeclarations);
	return QueryParserResult(parsedDeclarations, parsedSelectObj,
		parsedSTCl.noSynClauses, parsedSTCl.commonSynClauses,
		parsedPTCl.noSynClauses, parsedPTCl.commonSynClauses,
		parsedWithCl.noSynClauses, parsedWithCl.commonSynClauses);
}
