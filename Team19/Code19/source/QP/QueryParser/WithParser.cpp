#include "WithParser.h"


WithClauses WithParser::parse(QuerySplitter::tokenized_clauses withClauses, std::vector<Declaration> declarations) {
	std::vector<WithClause> noCommSynWClauses;
	std::vector<WithClause> hasCommSynWClauses;
	if (withClauses.empty()) {
		std::vector<WithClause> emptyCl = {};
		struct WithClauses withCl = { emptyCl, emptyCl };
		return withCl;
	}

	for (auto withClause : withClauses) {
		WithInfo parseWClRes = parseWithClause(withClause, declarations);
		if (parseWClRes.varsUsed.empty()) {
			noCommSynWClauses.push_back(parseWClRes.clauseObj);
		}
		else {
			hasCommSynWClauses.push_back(parseWClRes.clauseObj);
		}
	}
	struct WithClauses wClauses = { noCommSynWClauses, hasCommSynWClauses };
	return wClauses;
}

WithInfo WithParser::parseWithClause(std::vector<Token> withClause, std::vector<Declaration> declarations)
{
	string withClauseStr = QueryUtil::stringFromTokens(withClause);
	auto compareDelimPos = withClauseStr.find(COMPARE_DELIM);
	if (compareDelimPos == -1) { throw QuerySyntaxException(""); }
	string lhsRefStr = withClauseStr.substr(0, compareDelimPos);
	string rhsRefStr = withClauseStr.substr(compareDelimPos + 1);
	if (!isValidSyntax(lhsRefStr, rhsRefStr, declarations)) { throw QuerySyntaxException(""); }
	Ref lhsRef = getRefFromStr(lhsRefStr, declarations);
	Ref rhsRef = getRefFromStr(rhsRefStr, declarations);
	if (!isValidSemantic(lhsRef, rhsRef)) { throw QuerySemanticException(""); }
	return createWithClause(lhsRef, rhsRef);
}

/**
* Returns With-Clause of correct type based off Ref.argType
*/
WithInfo WithParser::createWithClause(Ref lhs, Ref rhs) {
	std::vector<Declaration> decsUsed;

	bool lhsIsAttrRef = std::holds_alternative<AttrRef>(lhs.value);
	bool rhsIsAttrRef = std::holds_alternative<AttrRef>(rhs.value);

	if (lhsIsAttrRef&& rhsIsAttrRef) {
		Declaration lhsDec = std::get<AttrRef>(lhs.value).synonym;
		Declaration rhsDec = std::get<AttrRef>(rhs.value).synonym;
		decsUsed = { lhsDec, rhsDec };
	}
	else if (lhsIsAttrRef) {
		Declaration lhsDec = std::get<AttrRef>(lhs.value).synonym;
		decsUsed = { lhsDec };
	}
	else if (rhsIsAttrRef) {
		Declaration rhsDec = std::get<AttrRef>(rhs.value).synonym;
		decsUsed = { rhsDec };
	}
	else {
		decsUsed = {};
	}
	struct Argument lhsArg = getArgFromRef(lhs);
	struct Argument rhsArg = getArgFromRef(rhs);
	WithClause wClause = WithClause(lhsArg, rhsArg);
	struct WithInfo wInfo = { decsUsed, wClause };
	return wInfo;
}

/**
* Returns true if with clause follows pattern: 
* a) ref '=' ref
* b) ref : '"' IDENT '"' | INTEGER | attrRef
*/
bool WithParser::isValidSyntax(std::string lhs, std::string rhs, std::vector<Declaration> declarations)
{
	return (QueryUtil::isValidWithRef(lhs, declarations))
		&& (QueryUtil::isValidWithRef(rhs, declarations));
}

/**
* Returns Ref object with argType and 
*/
Ref WithParser::getRefFromStr(std::string refStr, std::vector<Declaration> declarations) {
	// handle case by case, if INT | QUOTE | ATTRREF, with ATTRREF as default
	if (QueryUtil::isInteger(refStr)) {
		struct Ref result = { ArgType::STMTNO, refStr };
		return result;
	}
	if (QueryUtil::isQuote(refStr)) {
		struct Ref result = { ArgType::QUOTE, refStr };
		return result;
	}
	AttrRef attrRef = QueryUtil::getAttrRefFromStr(refStr, declarations);
	// if attrRef name contains NAME type attrRef
	if (attrRef.isNameType()) {
		struct Ref result = { ArgType::ATTR_REF_NAME, attrRef };
		return result;
	}
	if (attrRef.isIntType()) {
		struct Ref result = { ArgType::ATTR_REF_INT, attrRef };
		return result;
	}
	throw QuerySyntaxException("");
}

/**
* Returns true if:
* 1. isValidSemanticAttrRef for every AttrRef used in with-clause
* 2. ArgType-s of Ref are the same 
* (whereby ATTR_REF_NAME is same as NAME, ATTR_REF_INT is same as INT)
*/
bool WithParser::isValidSemantic(Ref lhs, Ref rhs) {
	bool lhsIsAttrRef = std::holds_alternative<AttrRef>(lhs.value);
	bool rhsIsAttrRef = std::holds_alternative<AttrRef>(rhs.value);

	if (lhsIsAttrRef && rhsIsAttrRef) {
		return QueryUtil::isValidSemanticAttrRef(std::get<AttrRef>(lhs.value))
			&& QueryUtil::isValidSemanticAttrRef(std::get<AttrRef>(rhs.value))
			&& (lhs.argType == rhs.argType);
	}
	else if (lhsIsAttrRef) {
		return QueryUtil::isValidSemanticAttrRef(std::get<AttrRef>(lhs.value))
			&& isAttrRefAndValueSameType(lhs, rhs);
	}
	else if (rhsIsAttrRef) {
		return QueryUtil::isValidSemanticAttrRef(std::get<AttrRef>(rhs.value))
			&& isAttrRefAndValueSameType(rhs, lhs);
	}
	else {
		return (lhs.argType == rhs.argType);
	}
}

/**
* Returns true if ArgType-s of Ref are the same
* (whereby ATTR_REF_NAME is same as NAME, ATTR_REF_INT is same as INT)
*/
bool WithParser::isAttrRefAndValueSameType(Ref ref_attrRef, Ref ref_value) {
	if (ref_value.argType == ArgType::QUOTE) {
		return (ref_attrRef.argType == ArgType::ATTR_REF_NAME);
	}
	else if (ref_value.argType == ArgType::STMTNO) {
		return (ref_attrRef.argType == ArgType::ATTR_REF_INT);
	}
	throw QuerySemanticException("");
}

/**
* Returns universal SynAttr type given a validated Ref type containing an AttrRef
*/
SynAttr WithParser::getSynAttrFromRef(Ref ref) {
	Declaration dec = std::get<AttrRef>(ref.value).synonym;
	ArgType argType = ref.argType;
	struct SynAttr synAttr = { dec, argType };
	return synAttr;
}

/**
* Returns Argument type given a Ref
*/
Argument WithParser::getArgFromRef(Ref ref) {
	struct Argument arg;
	ArgType argType = ref.argType;
	if (argType == ArgType::STMTNO) {
		return { Integer(std::get<std::string>(ref.value)) };
	}
	if (argType == ArgType::QUOTE) {
		std::string stringExp = QueryUtil::removeQuotes(std::get<std::string>(ref.value));
		return { StringExp(stringExp) };
	}
	return { getSynAttrFromRef(ref) };
}