#include "./GetIfPatternRequest.h"

GetIfPatternRequest::GetIfPatternRequest(PatternMode mode, Arg arg1, Arg arg2) : GetPatternRequest(PatternType::IF_PATTERN, mode, arg1, arg2) {}


PatternResults GetIfPatternRequest::execute(PatternStorage& patternStorage) const {
	PatternMode mode = getMode();
	IfPatternStorage storage = patternStorage.getIfStorage();
	switch (mode) {
	case PatternMode::ARG1:
		return storage.getIfByArg1(getArg1());
	default:
		return storage.getAllIfPtn();
	}
}


