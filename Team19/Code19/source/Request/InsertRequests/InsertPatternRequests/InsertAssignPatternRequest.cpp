#include "InsertAssignPatternRequest.h"

InsertAssignPatternRequest::InsertAssignPatternRequest(Ptn::AssignPtn assignPattern) :
	assignPattern(assignPattern) {}

Ptn::AssignPtn InsertAssignPatternRequest::getAssignPattern() const {
	return assignPattern;
}

void InsertAssignPatternRequest::execute(PatternStorage& patternStorage) const {
	patternStorage.insertAssign(assignPattern);
}

