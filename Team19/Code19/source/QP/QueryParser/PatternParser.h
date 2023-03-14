#pragma once
#include <vector>
#include <sstream>
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/Token.h"
#include "../QuerySplitter/QuerySplitter.h"
#include "../QueryObject/PatternClause.h"
#include "../QueryObject/Declaration.h"
#include "../QueryExceptions/QuerySemanticException.h"
#include "../QueryExceptions/QuerySyntaxException.h"
#include "../../SourceProcessor/Validator/ValidatorUtil.h"
#include "../QueryEvaluator/QueryUtil.h"

struct PatternInfo {
	std::vector<Declaration> varsUsed;
	PatternClause clauseObj;
};

struct PatternClauses {
	std::vector<PatternClause> noSynClauses;
	std::vector<PatternClause> commonSynClauses;

	bool PatternClauses::operator==(const PatternClauses& ptClauses) const {
		return tie(noSynClauses, commonSynClauses) == tie(ptClauses.noSynClauses, ptClauses.commonSynClauses);
	}
};

struct PatternParser {
public:
	static PatternClauses parse(QuerySplitter::tokenized_clauses patternClauses, std::vector<Declaration> declarations);
private:
	static TokenIterator head;
	static TokenIterator end;

	static PatternInfo parsePatternClause(std::vector<Token> patternClause, std::vector<Declaration> declarations);
	static std::vector<Declaration> getVarsUsed(PatternClause patternClause);
	static bool isValidSyntax(string arg1, string arg2, string arg3, std::vector<Declaration> declarations);
	static bool isValidIfSyntax(string arg1, string arg2, string arg3, std::vector<Declaration> declarations);
	static bool isValidAssignWhileSyntax(string arg1, string arg2, std::vector<Declaration> declarations);
	static bool isPatternSynValidSemantic(Declaration patternDec, string arg1, string arg2, string arg3, std::vector<Declaration> declarations);
	static PatternClause createPatternClause(Declaration patternDec, string arg1, string arg2, string arg3, std::vector<Declaration> declarations);
	static bool isPatternArgsValidSemantic(PatternClause patternCl);
	static bool isFirstArgValidSemantic(PatternClause patternCl);
};