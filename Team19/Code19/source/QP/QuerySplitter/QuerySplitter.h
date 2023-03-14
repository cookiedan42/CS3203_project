#pragma once
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/SPTokenizer.h"
#include <unordered_map>
#include <algorithm>
#include "../QueryExceptions/QuerySemanticException.h"
#include "../QueryExceptions/QuerySyntaxException.h"
#include "../../SourceProcessor/Validator/ValidatorUtil.h"
#include "../QueryEvaluator/QueryUtil.h"


struct QuerySplitter {
public:
	typedef std::string synName_t;
	typedef std::string synType_t;
	typedef std::vector<std::vector<Token>> tokenized_clauses;
	typedef pair<synName_t, synType_t> dec_pair;
private:
	std::vector<Token> tokens;
	TokenIterator head = TokenIterator::cbegin(tokens);
	TokenIterator end = TokenIterator::cend(tokens);

	// name, type
	std::vector<dec_pair> declarations {};
	std::vector<Token> selectArg {};
	tokenized_clauses suchThatClauses {};
	tokenized_clauses patternClauses {};
	tokenized_clauses withClauses {};

public:
	static std::vector<Token> tokenize(const std::string& arg) {
		return TokenSplitter(arg).tokenize();
	}
	static void validateHasSelect(const std::string& arg);

	QuerySplitter(std::vector<Token> tokens) : 
		// not typeDef-ed bc inaccurate use of type, primitive seems most accurate and still readable
		tokens(tokens) {}

	//wrapper
	void QuerySplitter::parse();

	//getters
	std::vector<dec_pair> getDeclarations() { return declarations; }
	std::vector<Token> getSelectArg() { return selectArg; }
	tokenized_clauses getSuchThatClauses() { return suchThatClauses; };
	tokenized_clauses getPatternClauses() { return patternClauses; };
	tokenized_clauses getWithClauses() { return withClauses; };

private:
	// first block is declarations
	void extractDeclarations();
	
	//	second block is Select <>
	void extractSelect();
	
	// third block is clauses
	void extractClauses();

	void extractSuchThatClause();
	void extractPatternClause();
	void extractWithClause();
	std::vector<Token> extractTuple();
	std::vector<Token> extractRef();
	std::vector<Token> extractAttrRef();
	bool isAttrRef(TokenIterator currTokeIt);
	void QuerySplitter::extractBetweenBrackets(std::vector<Token>& arg);
};
