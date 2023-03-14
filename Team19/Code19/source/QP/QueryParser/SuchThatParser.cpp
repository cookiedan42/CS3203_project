#include "SuchThatParser.h"

SuchThatClauses SuchThatParser::parse(QuerySplitter::tokenized_clauses suchThatClauses, std::vector<Declaration> declarations) {
	if (suchThatClauses.empty()) {
		std::vector<SuchThatClause> emptyCl = {};
		struct SuchThatClauses stClauses = { emptyCl, emptyCl };
		return stClauses;
	}
	std::vector<SuchThatClause> noCommSynSTClauses;
	std::vector<SuchThatClause> hasCommSynSTClauses;
	for (const auto& suchThatClause : suchThatClauses) {
		SuchThatInfo parseStClRes = parseSuchThatClause(suchThatClause, declarations);
		if (parseStClRes.varsUsed.empty()) {
			noCommSynSTClauses.push_back(parseStClRes.clauseObj);
		}
		else {
			hasCommSynSTClauses.push_back(parseStClRes.clauseObj);
		}
	}
	struct SuchThatClauses stClauses = { noCommSynSTClauses, hasCommSynSTClauses };
	return stClauses;
}

SuchThatInfo SuchThatParser::parseSuchThatClause(std::vector<Token> suchThatClause, std::vector<Declaration> declarations) {
	auto head = TokenIterator::cbegin(suchThatClause);
	auto end = TokenIterator::cend(suchThatClause);
	stringstream relation;
	stringstream ent1;
	stringstream ent2;

	while (!head.valueIs(BRACKET_START_DELIMITER)) {
		relation << head.getValue();
		head.advance();
	}
	head.advance();
	while (!head.valueIs(DEC_MULT_VAR_DELIMITER)) {
		ent1 << head.getValue();
		head.advance();
	}
	head.advance();
	while (!head.valueIs(BRACKET_END_DELIMITER)) {
		ent2 << head.getValue();
		head.advance();
	}
	head.advance();
	if (head != end) { throw QuerySyntaxException(""); }

	const std::string relation_str = relation.str();
	const std::string ent1_str = ent1.str();
	const std::string ent2_str = ent2.str();


	if (relation_str.empty()|| ent1_str.empty()|| ent2_str.empty()) { throw QuerySyntaxException(""); }
	if (!isValidSyntax(relation_str, ent1_str, ent2_str, declarations)) { throw QuerySyntaxException(""); }
	Argument ent1Arg = QueryUtil::getArgFromStr(ent1_str, declarations);
	Argument ent2Arg = QueryUtil::getArgFromStr(ent2_str, declarations);
	SuchThatClause stCl = SuchThatClause(DesignAbstractionFactory::createDesignAbstraction(relation_str, ent1Arg, ent2Arg));
	if (!isValidSemantic(stCl, declarations)) { throw QuerySemanticException(""); }
	std::vector<Declaration> varsInUse = getVarsUsed(stCl);
	struct SuchThatInfo stInfo = { varsInUse, stCl };
	return stInfo;
}

/**
* Returns all declarations used in pattern clause
*/
std::vector<Declaration> SuchThatParser::getVarsUsed(SuchThatClause suchThatClause) {
	std::vector<Declaration> decList = {};
	const auto& ent1 = suchThatClause.getArg1();
	const auto& ent2 = suchThatClause.getArg2();
	if (entityIsDeclaration(ent1)) {
		decList.push_back(std::get<Declaration>(ent1));
	}
	if (entityIsDeclaration(ent2)) {
		decList.push_back(std::get<Declaration>(ent2));
	}
	return decList;
}

/**
* Check if such-that clause follows relRef
*/
bool SuchThatParser::isValidSyntax(std::string relStr, std::string ent1, std::string ent2, std::vector<Declaration> declarations) {

	if (std::find(STMT_PAIR_REL.cbegin(), STMT_PAIR_REL.cend(), relStr) != STMT_PAIR_REL.cend()) {
		return QueryUtil::isValidStmtRef(ent1, declarations)
			&& QueryUtil::isValidStmtRef(ent2, declarations);
	}
	if ((relStr == USES_REL) || (relStr == MODIFIES_REL)) {
		return (QueryUtil::isValidEntRef(ent1, declarations) || QueryUtil::isValidStmtRef(ent1, declarations))
			&& QueryUtil::isValidEntRef(ent2, declarations);
	}
	if ((relStr == CALLS_REL) || (relStr == CALLS_T_REL)) {
		return QueryUtil::isValidEntRef(ent1, declarations)
			&& QueryUtil::isValidEntRef(ent2, declarations);
	}
	return false;
}

/**
* Checks if, given rel type, semantic is valid
*/
bool SuchThatParser::isValidSemantic(SuchThatClause suchThatClause, std::vector<Declaration> declarations) {

	const std::string relType = suchThatClause.relationship->type;

	if ((relType == USES_REL)
		|| (relType == MODIFIES_REL)) {
		return isValidSemantic_UsesModifies(suchThatClause, declarations);
	}

	if (std::find(STMT_PAIR_REL.cbegin(), STMT_PAIR_REL.cend(), relType) != STMT_PAIR_REL.cend()) {
		return isValidSemantic_StmtPair(suchThatClause, declarations);
	}

	if ((relType == CALLS_REL)
		|| (relType == CALLS_T_REL)) {
		return isValidSemantic_Calls(suchThatClause, declarations);
	}

	return true;
}

/**
* Checks that either:
*	'Uses'|'Modifies' '(' stmtRef ',' entRef ')'
* 'Uses'|'Modifies' '(' entRef ',' entRef ')'
*/
bool SuchThatParser::isValidSemantic_UsesModifies(SuchThatClause suchThatClause, std::vector<Declaration> declarations) {
	const auto& ent1 = suchThatClause.getArg1();

	if (std::holds_alternative<WildCard>(ent1)) { return false; }
	if (entityIsDeclaration(ent1)) {
		const auto& ent1Type = getDETypeFromEntity(ent1);
		if ((ent1Type == VARIABLETYPE) || (ent1Type == CONSTANTTYPE)) {
			return false;
		}
	}

	const auto& ent2 = suchThatClause.getArg2();
	if (entityIsDeclaration(ent2)) {
		const auto& ent2Type = getDETypeFromEntity(ent2);
		if (ent2Type != VARIABLETYPE) {
			return false;
		}
	}

	return true;
}

/**
* Checks that follows:
* 'Calls' | 'Calls*' '(' entRef ',' entRef ')'
*/
bool SuchThatParser::isValidSemantic_Calls(SuchThatClause suchThatClause, std::vector<Declaration> declarations) {

	const auto& ent1 = suchThatClause.getArg1();
	if (entityIsDeclaration(ent1)) {
		const auto& ent1Type = getDETypeFromEntity(ent1);
		if (ent1Type != PROCEDURETYPE) {
			return false;
		}
	}

	const auto& ent2 = suchThatClause.getArg2();
	if (entityIsDeclaration(ent2)) {
		const auto& ent2Type = getDETypeFromEntity(ent2);
		if (ent2Type != PROCEDURETYPE) {
			return false;
		}
	}

	return true;
}

/**
* Checks that follows: '(' stmtRef ',' stmtRef ')'
*/
bool SuchThatParser::isValidSemantic_StmtPair(SuchThatClause suchThatClause, std::vector<Declaration> declarations) {
	auto ent1 = suchThatClause.getArg1();
	if (entityIsDeclaration(ent1)) {
		auto ent1Type = getDETypeFromEntity(ent1);
		if ((ent1Type == VARIABLETYPE) || (ent1Type == CONSTANTTYPE) || (ent1Type == PROCEDURETYPE)) {
			return false;
		}
	}
	auto ent2 = suchThatClause.getArg2();
	if (entityIsDeclaration(ent2)) {
		auto ent2Type = getDETypeFromEntity(ent2);
		if ((ent2Type == VARIABLETYPE) || (ent2Type == CONSTANTTYPE) || (ent2Type == PROCEDURETYPE)) {
			return false;
		}
	}
	return true;

}

/**
* Given that entity is a declaration, return its DE type
*/
string SuchThatParser::getDETypeFromEntity(Argument::Arg arg) {
	return std::get<Declaration>(arg).synType->type;
}


/**
* Returns true if entity is a declaration
*/
bool SuchThatParser::entityIsDeclaration(Argument::Arg arg) {
	return std::holds_alternative<Declaration>(arg);
}