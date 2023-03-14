#include "InsertWhilePatternRequest.h"

InsertWhilePatternRequest::InsertWhilePatternRequest(Ptn::WhilePtn whilePattern) :
	whilePattern(whilePattern) {}

Ptn::WhilePtn InsertWhilePatternRequest::getWhilePattern() const {
	return whilePattern;
}

void InsertWhilePatternRequest::execute(PatternStorage& patternStorage) const {
	patternStorage.insertWhile(whilePattern);
}

