#include "AffectsTEvaluator.h"

AffectsTEvaluator::AffectsTEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

// Affects*(int,int), Affects*(int,_), Affects*(_,int), Affects*(_,_)
bool AffectsTEvaluator::evaluateClauseWithNoSyn() {
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);

	if (type == ArgsType::STMTNO_STMTNO) { // e.g. Affects*(1,2)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		stmtNo_t start = stoi(arg1Value);
		stmtNo_t end = stoi(arg2Value);

		auto hasAffectsT = checkSpecificAffectsT(start, end);
		return hasAffectsT;
	}
	else { // Affects*(int,_), Affects*(_,int), Affects*(_,_) can be reduced to Affects
		return AffectsEvaluator(arg1, arg2, facade).evaluateClauseWithNoSyn();
	}
}

// Affects*(syn,syn), Affects*(syn,int), Affects*(int,syn), Affects*(syn,_), Affects*(_,syn)
RelationshipResults AffectsTEvaluator::getRelationPairs() {
	RelationshipResults results;

	ArgType arg1Type = QueryUtil::getArgTypeFromArgument(arg1);
	ArgType arg2Type = QueryUtil::getArgTypeFromArgument(arg2);
	bool isArg1Syn = arg1Type == ArgType::SYN && arg2Type != ArgType::SYN;
	bool isArg2Syn = arg2Type == ArgType::SYN && arg1Type != ArgType::SYN;
	// if syn types are wrong, return empty table!
	bool arg1_synValid = !isArg1Syn || isValidSynType(arg1);
	bool arg2_synValid = !isArg2Syn || isValidSynType(arg2);

	if (!arg1_synValid || !arg2_synValid) return results;

	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);
	if (type == ArgsType::SYN_WILDCARD || type == ArgsType::WILDCARD_SYN) { // Affects*(s,_), Affects*(_,s) reduced to Affects
		auto& cfgs = AffectsUtil::getAllCFGs(facade);

		for (const auto& cfg : cfgs) {
			auto res = AffectsSynSyn(facade, *cfg).evaluate();
			results.insert(res.cbegin(), res.cend());
		}
	}
	else if (type == ArgsType::STMTNO_SYN) { // e.g. Affects*(1, s)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		stmtNo_t start = stoi(arg1Value);

		results = getAffectedTStatements(start);
	}
	else if (type == ArgsType::SYN_STMTNO) { // e.g. Affects*(s,2)
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		stmtNo_t end = stoi(arg2Value);

		results = getAffectingTStatements(end);
	}
	else { // e.g. Affects*(s1,s2)
		results = getAllAffectsT();
	}

	if (type == ArgsType::SYN_SYN && QueryUtil::isSameSynonym(arg1, arg2)) {
		results = QueryUtil::filterEqualPairs(results);
	}

	return results;
}

// Some affects* are affects in disguise:
// Affects*(_,_), Affects*(int,_), Affects*(_,int), Affects*(syn,_), Affects*(_syn)

// others are ... not so trivial

// Affects*(1,1)
bool AffectsTEvaluator::checkSpecificAffectsT(const stmtNo_t& start, const stmtNo_t& end) {
	auto opt = AffectsUtil::getCommonCFG(facade, start, end);
	if (!opt.has_value()) return false; // Statements in different CFGs

	const auto& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, start)) return false;
	if (!AffectsUtil::isAssignStatement(cfg, end)) return false;

	if (!cfg.hasPath_node(start, end)) return false;

	bool hasAffects = AffectsStmtStmt(facade, cfg, start, end).evaluate();
	if (hasAffects) return true;

	bool hasAffectsT = AffectsTStmtStmt(facade, cfg, end).evaluate(start, end);
	return hasAffectsT;
}

// Affects*(a,1)
RelationshipResults AffectsTEvaluator::getAffectingTStatements(stmtNo_t target) {
	auto opt = AffectsUtil::getCFGByStmtNo(facade, target);
	if (!opt.has_value()) return {}; // no matching CFG

	Cfg& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, target)) return {};

	return AffectsTSynStmt(facade, cfg).evaluate(target);
}

// Affects*(1,a)
RelationshipResults AffectsTEvaluator::getAffectedTStatements(stmtNo_t target) {
	auto opt = AffectsUtil::getCFGByStmtNo(facade, target);
	if (!opt.has_value()) return {}; // no matching CFG

	Cfg& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, target)) return {};

	return AffectsTStmtSyn(facade, cfg).evaluate(target);
}

// Affects*(a,b)
RelationshipResults AffectsTEvaluator::getAllAffectsT() {
	RelationshipResults results = AffectsTSynSyn(facade).evaluate();
	return results;
}

bool AffectsTEvaluator::isValidSynType(Argument arg) {
	Declaration dec = std::get<Declaration>(arg.argumentVariant);
	return dec.synType->type == STATEMENT_DE
		|| dec.synType->type == ASSIGN_DE;
}
