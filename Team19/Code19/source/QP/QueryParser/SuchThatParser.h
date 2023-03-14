#pragma once
#include <vector>
#include <variant>
#include <sstream>
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/Token.h"
#include "../QuerySplitter/QuerySplitter.h"
#include "../QueryObject/SuchThatClause.h"
#include "../QueryObject/Declaration.h"
#include "../QueryExceptions/QuerySemanticException.h"
#include "../QueryExceptions/QuerySyntaxException.h"
#include "../../SourceProcessor/Validator/ValidatorUtil.h"
#include "../QueryEvaluator/QueryUtil.h"
#include "../QueryObject/DesignAbstractionFactory.h"

struct SuchThatInfo {
	std::vector<Declaration> varsUsed;
	SuchThatClause clauseObj;
};

struct SuchThatClauses {
	std::vector<SuchThatClause> noSynClauses;
	std::vector<SuchThatClause> commonSynClauses;
};

struct SuchThatParser {
public:
	static SuchThatClauses parse(QuerySplitter::tokenized_clauses suchThatClauses, std::vector<Declaration> declarations);
private:

	static SuchThatInfo parseSuchThatClause(std::vector<Token> suchThatClause, std::vector<Declaration> declarations);
	static bool isValidSyntax(std::string relStr, std::string ent1, std::string ent2, std::vector<Declaration> declarations);
	static bool isValidSemantic(SuchThatClause suchThatClause, std::vector<Declaration> declarations);
	static bool isValidSemantic_UsesModifies(SuchThatClause suchThatClause, std::vector<Declaration> declarations);
	static bool isValidSemantic_Calls(SuchThatClause suchThatClause, std::vector<Declaration> declarations);
	static bool isValidSemantic_StmtPair(SuchThatClause suchThatClause, std::vector<Declaration> declarations);
	static bool entityIsDeclaration(Argument::Arg arg);
	static string getDETypeFromEntity(Argument::Arg arg);
	
	static std::vector<Declaration> getVarsUsed(SuchThatClause suchThatClause);
};