#include "SPManager.h"

SPManager SPManager::construct(std::string source) {
	auto tokens = TokenSplitter(source).tokenize();
	try {
		Validator(tokens).validateProgram();
	} catch (const std::exception& exp) {
		throw exp;
	}
	auto ast = Parser(tokens).parseTokens();
	auto de = DesignExtractor(move(ast));
	return SPManager(move(de));
}

void SPManager::populatePKB(InsertFacade& facade) {
	// Sends requests to PKB for insertion
	for (const auto& entityRequest : de.getEntityRequests()) facade.insert(entityRequest);

	for (const auto& relationshipRequest : de.getRelationshipRequests()) facade.insert(relationshipRequest);

	for (const auto& patternRequest : de.getPatternRequests()) facade.insert(patternRequest);

	for (const auto& cfg : de.getCFG()) facade.insertCFG(cfg);

	facade.generateSecondaryData();
}
