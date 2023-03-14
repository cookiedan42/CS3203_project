#include "PatternManager.h"


PatternManager::PatternManager() {
	patternStorage = new PatternStorage;
}

void PatternManager::insert(std::shared_ptr<InsertPatternRequest> request) {
	request->execute(*patternStorage);
}

PatternResults PatternManager::get(std::shared_ptr<GetPatternRequest> request) {
	return request->execute(*patternStorage);
}

