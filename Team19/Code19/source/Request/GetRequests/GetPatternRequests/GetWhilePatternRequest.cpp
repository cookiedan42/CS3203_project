#include "./GetWhilePatternRequest.h"

GetWhilePatternRequest::GetWhilePatternRequest(PatternMode mode, Arg arg1, Arg arg2) : GetPatternRequest(PatternType::WHILE_PATTERN, mode, arg1, arg2) {}

PatternResults GetWhilePatternRequest::execute(PatternStorage& patternStorage) const {
	PatternMode mode = getMode();
	WhilePatternStorage storage = patternStorage.getWhileStorage();
	switch (mode) {
	case PatternMode::ARG1:
		return storage.getWhileByArg1(getArg1());
	default:
		return storage.getAllWhilePtn();
	}
}