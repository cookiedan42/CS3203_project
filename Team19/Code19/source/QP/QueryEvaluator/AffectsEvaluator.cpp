#include "AffectsEvaluator.h"

AffectsEvaluator::AffectsEvaluator(Argument& arg1, Argument& arg2, GetFacade& facade) :
	RelationshipEvaluator(arg1, arg2, facade) {}

// Affects(int,int), Affects(int,_), Affects(_,int), Affects(_,_)
bool AffectsEvaluator::evaluateClauseWithNoSyn() {
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);

	if (type == ArgsType::STMTNO_STMTNO) { // e.g. Affects(1, 2)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		stmtNo_t start = stoi(arg1Value);
		stmtNo_t end = stoi(arg2Value);

		auto hasAffects = checkSpecificAffects(start, end);
		return hasAffects;
	}
	else if (type == ArgsType::STMTNO_WILDCARD) { // e.g. Affects(1,_)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		stmtNo_t start = stoi(arg1Value);

		auto hasAffects = checkIfAffects(start);
		return hasAffects;
	}
	else if (type == ArgsType::WILDCARD_STMTNO) { // e.g. Affects(_,10)
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		stmtNo_t end = stoi(arg2Value);

		auto hasAffects = checkIfAffected(end);
		return hasAffects;
	}
	else { // Affects(_,_)
		auto hasAffects = checkIfAffectsExists();
		return hasAffects;
	}
}

// Affects(syn,syn), Affects(syn,int), Affects(int,syn), Affects(syn,_), Affects(_,syn)
RelationshipResults AffectsEvaluator::getRelationPairs() {
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
	if (type == ArgsType::STMTNO_SYN) { // e.g. Affects(1, s)
		ArgString arg1Value = std::get<StmtNo>(arg1.argumentVariant).value;
		stmtNo_t start = stoi(arg1Value);

		results = getAffectedStatements(start);
	}
	else if (type == ArgsType::SYN_STMTNO) { // e.g. Affects(s,2)
		ArgString arg2Value = std::get<StmtNo>(arg2.argumentVariant).value;
		stmtNo_t end = stoi(arg2Value);

		results = getAffectingStatements(end);
	}
	else { // e.g. Affects(s1,s2), Affects(s,_), Affects(_,s)
		results = getAllAffects();
	}

	if (type == ArgsType::SYN_SYN && QueryUtil::isSameSynonym(arg1, arg2)) {
		results = QueryUtil::filterEqualPairs(results);
	}

	return results;
}

// Affects(int,int)
bool AffectsEvaluator::checkSpecificAffects(const stmtNo_t& start, const stmtNo_t& end) {
	auto opt = AffectsUtil::getCommonCFG(facade, start, end);
	if (!opt.has_value()) return false; // Statements in different CFGs

	const auto& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, start)) return false;
	if (!AffectsUtil::isAssignStatement(cfg, end)) return false;

	if (!cfg.hasPath(start, end)) return false;

	bool hasAffects = AffectsStmtStmt(facade, cfg, start, end).evaluate();
	return hasAffects;
}

// Affects(int,_)
bool AffectsEvaluator::checkIfAffects(stmtNo_t target) {
	auto opt = AffectsUtil::getCFGByStmtNo(facade, target);
	if (!opt.has_value()) return false; // Invalid stmtNo

	auto& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, target)) return false;

	bool hasAffects = AffectsStmtWild(facade, cfg, target).evaluate();
	return hasAffects;
}

// Affects(_,int)
bool AffectsEvaluator::checkIfAffected(stmtNo_t target) {
	auto opt = AffectsUtil::getCFGByStmtNo(facade, target);
	if (!opt.has_value()) return false; // Invalid stmtNo

	auto& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, target)) return false;

	bool hasAffected = AffectsWildStmt(facade, cfg, target).evaluate();
	return hasAffected;
}

// Affects(_,_)
bool AffectsEvaluator::checkIfAffectsExists() {
	auto& cfgs = AffectsUtil::getAllCFGs(facade);

	for (const auto& cfg : cfgs) {
		auto res = AffectsWildWild(facade,*cfg).evaluate();
		if (res) return true; // if any affects exists --> return true
	}

	return false;
}

// Affects(int,syn)
RelationshipResults AffectsEvaluator::getAffectedStatements(stmtNo_t target) {
	auto opt = AffectsUtil::getCFGByStmtNo(facade, target);
	if (!opt.has_value()) return {}; // Invalid stmtNo

	auto& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, target)) return {};

	RelationshipResults results = AffectsStmtSyn(facade, cfg, target).evaluate();
	return results;
}

// Affects(syn,int)
RelationshipResults AffectsEvaluator::getAffectingStatements(stmtNo_t target) {
	auto opt = AffectsUtil::getCFGByStmtNo(facade, target);
	if (!opt.has_value()) return {}; // Invalid stmtNo

	auto& cfg = opt.value();

	if (!AffectsUtil::isAssignStatement(cfg, target)) return {};

	RelationshipResults results = AffectsSynStmt(facade, cfg, target).evaluate();
	return results;
}

// Affects(syn,syn)
RelationshipResults AffectsEvaluator::getAllAffects() {
	RelationshipResults results{};

	auto& cfgs = AffectsUtil::getAllCFGs(facade);

	for (const auto& cfg : cfgs) {
		auto res = AffectsSynSyn(facade, *cfg).evaluate();
		results.insert(res.cbegin(), res.cend());
	}

	return results;
}

bool AffectsEvaluator::isValidSynType(Argument arg) {
	Declaration dec = std::get<Declaration>(arg.argumentVariant);
	return dec.synType->type == STATEMENT_DE 
		|| dec.synType->type == ASSIGN_DE;
}


