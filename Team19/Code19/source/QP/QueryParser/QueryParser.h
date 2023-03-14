#pragma once
#include <unordered_map>
#include <vector>
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/Token.h"
#include "../QuerySplitter/QuerySplitter.h"
#include "../QueryObject/QueryObject.h"
#include "../QueryExceptions/QuerySemanticException.h"
#include "../QueryExceptions/QuerySyntaxException.h"

#include "DeclarationsParser.h"
#include "SelectParser.h"
#include "SuchThatParser.h"
#include "PatternParser.h"
#include "WithParser.h"

#include "./QueryParserResult.h"


struct QueryParser {
public:
	QueryParser(
		std::vector<QuerySplitter::dec_pair> declarations,
		std::vector<Token> selectArg,
		QuerySplitter::tokenized_clauses suchThatClauses,
		QuerySplitter::tokenized_clauses patternClauses,
		QuerySplitter::tokenized_clauses withClauses
	) : declarations(declarations), selectArg(selectArg),
		suchThatClauses(suchThatClauses), patternClauses(patternClauses),
		withClauses(withClauses) {};

private:
	std::vector<QuerySplitter::dec_pair> declarations;
	std::vector<Token> selectArg{};
	QuerySplitter::tokenized_clauses suchThatClauses{};
	QuerySplitter::tokenized_clauses patternClauses{};
	QuerySplitter::tokenized_clauses withClauses{};

	std::vector<Declaration> parsedDeclarations;
	SelectObject parsedSelectObj;
	SuchThatClauses parsedSTCl;
	PatternClauses parsedPTCl;
	WithClauses parsedWithCl;

	std::vector<Declaration> parseDeclarations(std::vector<QuerySplitter::dec_pair> declarations);
	SelectObject parseSelectObj(std::vector<Token> selectArg, std::vector<Declaration> declarations);
	SuchThatClauses parseSuchThatClauses(QuerySplitter::tokenized_clauses suchThatClauses, std::vector<Declaration> declarations);
	PatternClauses parsePatternClauses(QuerySplitter::tokenized_clauses patternClauses, std::vector<Declaration> declarations);
	WithClauses parseWithClauses(QuerySplitter::tokenized_clauses withClauses, std::vector<Declaration> declarations);

public:
	QueryParserResult QueryParser::parseQuery();
};