#include "PatternParser.h"

PatternClauses PatternParser::parse(QuerySplitter::tokenized_clauses patternClauses, std::vector<Declaration> declarations) {
	std::vector<PatternClause> noCommSynPTClauses;
	std::vector<PatternClause> hasCommSynPTClauses;
	if (patternClauses.empty()) {
		std::vector<PatternClause> emptyCl = {};
		struct PatternClauses ptClauses = { emptyCl, emptyCl };
		return ptClauses;
	}
	
	for (const auto& patternClause : patternClauses) {
	PatternInfo parsePtClRes = parsePatternClause(patternClause, declarations);
		if (parsePtClRes.varsUsed.empty()) {
			noCommSynPTClauses.push_back(parsePtClRes.clauseObj);
		}
		else {
			hasCommSynPTClauses.push_back(parsePtClRes.clauseObj);
		}
	}
	struct PatternClauses ptClauses = { noCommSynPTClauses, hasCommSynPTClauses };
	return ptClauses;
}

PatternInfo PatternParser::parsePatternClause(std::vector<Token> patternClause, std::vector<Declaration> declarations) {
	if (patternClause.size() < 6) { throw QuerySyntaxException(""); }		// minimal pattern is: syn ( _ , _ )
	
	auto last = TokenIterator::cend(patternClause) - 1;
	if (!last.valueIs(BRACKET_END_DELIMITER)) { throw QuerySyntaxException(""); }

	auto head = TokenIterator::cbegin(patternClause);
	Token syn = head.getToken();
	head.advance();
	if (!head.valueIs(BRACKET_START_DELIMITER)) { throw QuerySyntaxException(""); }
	head.advance();

	std::vector<Token> arg1 {};
	std::vector<Token> arg2 {};
	std::vector<Token> arg3 {};
	
	while (!head.valueIs(",") && head != last) {
		arg1.push_back(head.getToken());
		head.advance();
	}
	if (arg1.size() == 0) { throw QuerySyntaxException(""); }
	if (!head.valueIs(",")) { throw QuerySyntaxException(""); }
	head.advance();

	while (!head.valueIs(",") && head != last) {
		arg2.push_back(head.getToken());
		head.advance();
	}
	if (arg2.size() == 0) { throw QuerySyntaxException(""); }

	if (head.valueIs(",")) {	// third arg exists
		head.advance();
		while (!head.valueIs(",") && head != last) {
			arg3.push_back(head.getToken());
			head.advance();
		}
		if (arg3.size() == 0) { throw QuerySyntaxException(""); }	
	} 

	if (head != last) { throw QuerySyntaxException(""); }

	string arg1Str = QueryUtil::stringFromTokens(arg1);
	string arg2Str = QueryUtil::stringFromTokens(arg2);
	string arg3Str = QueryUtil::stringFromTokens(arg3);

	if (!isValidSyntax(arg1Str, arg2Str, arg3Str, declarations)) { throw QuerySyntaxException(""); }
	optional<Declaration> synRef = QueryUtil::getFromDeclarations(syn.getValue(), declarations);
	if (!synRef) { throw QuerySemanticException(""); }
	Declaration synDec = synRef.value();

	if (!isPatternSynValidSemantic(synDec, arg1Str, arg2Str, arg3Str, declarations)) { throw QuerySemanticException(""); }
	PatternClause ptCl = createPatternClause(synDec, arg1Str, arg2Str, arg3Str, declarations);
	if (!isPatternArgsValidSemantic(ptCl)) { throw QuerySemanticException(""); }
	std::vector<Declaration> varsInUse = getVarsUsed(ptCl);
	struct PatternInfo ptInfo = { varsInUse, ptCl };
	return ptInfo;
}

/**
* Returns all declarations used in pattern clause
*/
std::vector<Declaration> PatternParser::getVarsUsed(PatternClause patternClause) {
	std::vector<Declaration> decList = {};
	decList.push_back(patternClause.synonym);
	auto arg1 = patternClause.arg_1.argumentVariant;
	if (std::holds_alternative<Declaration>(arg1)) {
		decList.push_back(std::get<Declaration>(arg1));
	}
	return decList;
}   

/**
* Check if pattern follows possible sequences:
* - syn '(' entRef ',' expression-spec ')'
* - syn '(' entRef ',' '_' ')'
* - syn '(' entRef ',' '_' ',' '_' ')'
*/
bool PatternParser::isValidSyntax(string arg1, string arg2, string arg3, std::vector<Declaration> declarations) {
	if (arg3 != "") { // if
		return isValidIfSyntax(arg1, arg2, arg3, declarations);
	}
	return isValidAssignWhileSyntax(arg1, arg2, declarations);
}

/**
* Check if follows: syn-if '(' entRef ',' '_' ',' '_' ')'
*/
bool PatternParser::isValidIfSyntax(string arg1, string arg2, string arg3, std::vector<Declaration> declarations) {
	return QueryUtil::isValidEntRef(arg1, declarations) &&
		QueryUtil::isWildcard(arg2) &&
		QueryUtil::isWildcard(arg3);
}

/**
* Check if follows: syn-assign | syn-while '(' entRef ',' expression-spec ')'
* note that syn-while syntax is subset of syn-assign valid syntax
*/
bool PatternParser::isValidAssignWhileSyntax(string arg1, string arg2, std::vector<Declaration> declarations) {
	return QueryUtil::isValidEntRef(arg1, declarations) &&
		QueryUtil::isValidExprSpec(arg2);
}

/**
* Check:
* - if 3 args, syn-if type
* - if 2nd arg is wildcard, syn-while or syn-assign
* - else, syn-assign
*/
bool PatternParser::isPatternSynValidSemantic(Declaration patternDec, string arg1, string arg2, string arg3, std::vector<Declaration> declarations) {
	auto synType = patternDec.synType->type;
	if (arg3 != "") { // if
		return synType == IF_DE;
	}
	if (QueryUtil::isWildcard(arg2)) { // assign | while
		return (synType == ASSIGN_DE) || 
			(synType == WHILE_DE);
	}
	// exprSpec at RHS -> has to be assign type
	return synType == ASSIGN_DE;
}

/**
* Create correct pattern clause based on pattern synonym's synType
*/
PatternClause PatternParser::createPatternClause(Declaration patternDec, string arg1, string arg2, string arg3, std::vector<Declaration> declarations) {
	auto synType = patternDec.synType->type;
	Argument argmt1 = QueryUtil::getArgFromStr(arg1, declarations);
	Argument argmt2 = QueryUtil::getArgFromStr(arg2, declarations);
	if (synType == IF_DE) {
		if (!std::holds_alternative<WildCard>(argmt2.argumentVariant)) { throw QuerySemanticException(""); }
		return PatternClause(patternDec, argmt1, argmt2);
	}
	else if (synType == WHILE_DE) {
		if (!std::holds_alternative<WildCard>(argmt2.argumentVariant)) { throw QuerySemanticException(""); }
		return PatternClause(patternDec, argmt1, argmt2);
	} 
	else if (synType == ASSIGN_DE) {
		return PatternClause(patternDec, argmt1, argmt2);
	}
	throw QuerySemanticException("");
}

/**
* Checks that arguments are valid
*/
bool PatternParser::isPatternArgsValidSemantic(PatternClause patternCl) {
	// if arg1 is Declaration, must be only variable type - for assign | while | if
	if (std::holds_alternative<Declaration>(patternCl.arg_1.argumentVariant)) {
		Declaration arg1Dec = std::get<Declaration>(patternCl.arg_1.argumentVariant);
		if (arg1Dec.synType->type != VARIABLE_DE) { return false; }
	}
	// if while | if, second arg has to be a wildcard
	auto patternSynType = patternCl.synonym.synType->type;
	if (patternCl.isWhilePattern() || patternCl.isIfPattern()) {
		if (!std::holds_alternative<WildCard>(patternCl.arg_2.argumentVariant)) { return false; }
	}
	return true;
}
