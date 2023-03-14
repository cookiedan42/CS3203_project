#include "InsertIfPatternRequest.h"

InsertIfPatternRequest::InsertIfPatternRequest(Ptn::IfPtn ifPattern) : ifPattern(ifPattern) {}

Ptn::IfPtn InsertIfPatternRequest::getIfPattern() const {
	return ifPattern;
}

void InsertIfPatternRequest::execute(PatternStorage& patternStorage) const {
	patternStorage.insertIf(ifPattern);
}

