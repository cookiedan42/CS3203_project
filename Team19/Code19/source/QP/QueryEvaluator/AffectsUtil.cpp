#include "AffectsUtil.h"

bool AffectsUtil::isAssignStatement(const Cfg& cfg, const stmtNo_t& stmtNo) {
	return cfg.getStmtType(stmtNo) == Ent::assign_stmtType;
}

bool AffectsUtil::isIfStatement(const Cfg& cfg, const stmtNo_t& stmtNo) {
	return cfg.getStmtType(stmtNo) == Ent::if_stmtType;
}

bool AffectsUtil::isWhileStatement(const Cfg& cfg, const stmtNo_t& stmtNo) {
	return cfg.getStmtType(stmtNo) == Ent::while_stmtType;
}

bool AffectsUtil::isNotContainerStatement(Cfg cfg, stmtNo_t current) {
	return cfg.getStmtType(current) != Ent::if_stmtType &&
		cfg.getStmtType(current) != Ent::while_stmtType;
}

void AffectsUtil::addToLMT(GetFacade& facade,const stmtNo_t& current, LatestModifiedTable& lmt) {
	auto modifiedVar = getVariableModified(facade, current);
	lmt[modifiedVar] = { current };
}

LatestModifiedTable AffectsUtil::mergeLMTs(LatestModifiedTable& lmt1, LatestModifiedTable& lmt2) {
	LatestModifiedTable lmt = lmt1;

	for (const auto& [modifiedVar, modifiers] : lmt2) {
		if (lmt.count(modifiedVar)) {
			auto& modifierSet = lmt.at(modifiedVar);
			modifierSet.insert(modifiers.cbegin(), modifiers.cend());
		} else {
			lmt.insert({ modifiedVar, modifiers });
		}
	}
	return lmt;
}

optional<Cfg> AffectsUtil::getCommonCFG(GetFacade& facade, const stmtNo_t& start, const stmtNo_t& end) {
	auto getCFGRequest = std::make_shared<GetCFGRequest>(start);
	auto cfgResults = facade.get(getCFGRequest);

	if (cfgResults.empty()) return nullopt; // Invalid stmtNo, cfg not found in pkb

	auto& cfg = *cfgResults.cbegin();

	if (cfg->contains(end)) return *cfg;

	return nullopt;
}

optional<Cfg> AffectsUtil::getCFGByStmtNo(GetFacade& facade, const stmtNo_t& stmtNo) {
	auto getCFGRequest = std::make_shared<GetCFGRequest>(stmtNo);
	auto cfgResults = facade.get(getCFGRequest);

	if (cfgResults.empty()) return nullopt;

	return **cfgResults.cbegin();
}

CFGResults AffectsUtil::getAllCFGs(GetFacade& facade) {
	auto cfgRequest = make_shared<GetCFGRequest>();
	auto cfgs = facade.get(cfgRequest);

	return cfgs;
}

optional<string> AffectsUtil::getModifiedAndUsed(GetFacade& facade, const stmtNo_t& arg1, const stmtNo_t& arg2) {
	
	const std::string variableModified = getVariableModified(facade, arg1);
	bool varIsUsed = checkStmtUses(facade, arg2, variableModified);

	if (varIsUsed) return variableModified;
	return nullopt;
}

unordered_set<string> AffectsUtil::getVariablesUsed(GetFacade& facade, const stmtNo_t& stmtNo) {
	auto request = std::make_shared<GetUsesRequest>(std::to_string(stmtNo), Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
	auto results = facade.get(request);

	unordered_set<string> variables;
	for (const auto& [_, var] : results) variables.insert(var);

	return variables;
}

// for non-container statement, it can only ever modify one statement
string AffectsUtil::getVariableModified(GetFacade& facade, const stmtNo_t& stmtNo) {
	auto request = std::make_shared<GetModifiesRequest>(std::to_string(stmtNo), Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
	auto results = facade.get(request);

	return results.cbegin()->second;
}

unordered_set<string> AffectsUtil::getAllVariablesModified(GetFacade& facade, const stmtNo_t& stmtNo) {
	auto request = std::make_shared<GetModifiesRequest>(std::to_string(stmtNo), Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
	RelationshipResults results = facade.get(request);
	
	unordered_set<string> out{};
	for (auto& [_, val] : results) out.insert(val);

	return out;
}

bool AffectsUtil::checkStmtUses(GetFacade& facade, const stmtNo_t& current, const std::string& commonVar) {
	auto request = std::make_shared<GetUsesRequest>
		(std::to_string(current), commonVar, Mode::FILTER_ARG1_ARG2, Arg1Type::STATEMENT);
	auto results = facade.get(request);
	return !results.empty();
}

bool AffectsUtil::checkStmtModifies(GetFacade& facade, const stmtNo_t& current, const std::string& commonVar) {
	auto request = std::make_shared<GetModifiesRequest>
		(std::to_string(current), commonVar, Mode::FILTER_ARG1_ARG2, Arg1Type::STATEMENT);
	auto results = facade.get(request);
	return !results.empty();
}
