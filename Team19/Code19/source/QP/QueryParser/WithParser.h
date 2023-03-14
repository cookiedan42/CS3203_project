#pragma once
#include <vector>
#include <variant>
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/Token.h"
#include "AttrRef.h"
#include "../QuerySplitter/QuerySplitter.h"
#include "../QueryObject/WithClause.h"
#include "../QueryObject/Declaration.h"
#include "../QueryObject/Variants/Integer.h"
#include "../QueryObject/Variants/StringExp.h"
#include "../QueryExceptions/QuerySemanticException.h"
#include "../QueryExceptions/QuerySyntaxException.h"
#include "../../SourceProcessor/Validator/ValidatorUtil.h"
#include "../QueryEvaluator/QueryUtil.h"

struct Ref {
	ArgType argType;
	std::variant<std::string, AttrRef> value;
};

struct WithInfo {
	std::vector<Declaration> varsUsed;
	WithClause clauseObj;
};

struct WithClauses {
	std::vector<WithClause> noSynClauses;
	std::vector<WithClause> commonSynClauses;

	bool WithClauses::operator==(const WithClauses& withClauses) const {
		return tie(noSynClauses, commonSynClauses) == tie(withClauses.noSynClauses, withClauses.commonSynClauses);
	}
};

struct WithParser {
public:
	static WithClauses parse(QuerySplitter::tokenized_clauses withClauses, std::vector<Declaration> declarations);
private:
	static WithInfo parseWithClause(std::vector<Token> withClause, std::vector<Declaration> declarations);
	static WithInfo createWithClause(Ref lhs, Ref rhs);
	static bool isValidSyntax(std::string lhs, std::string rhs, std::vector<Declaration> declarations);
	static Ref getRefFromStr(std::string refStr, std::vector<Declaration> declarations);
	static bool isValidSemantic(Ref lhs, Ref rhs);
	static bool isAttrRefAndValueSameType(Ref ref_attrRef, Ref ref_value);
	static SynAttr getSynAttrFromRef(Ref ref);
	static Argument getArgFromRef(Ref ref);
};