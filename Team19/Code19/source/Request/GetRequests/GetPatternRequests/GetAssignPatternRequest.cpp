#include "./GetAssignPatternRequest.h"


GetAssignPatternRequest::GetAssignPatternRequest(PatternMode mode, Arg arg1, Arg arg2) : GetPatternRequest(PatternType::ASSIGNMENT_PATTERN, mode, arg1, arg2) {}

PatternResults GetAssignPatternRequest::execute(PatternStorage& patternStorage) const {

	PatternMode mode = getMode();
	AssignPatternStorage storage = patternStorage.getAssignStorage();
	switch (mode) {
	case PatternMode::NO_ARG:
		return storage.getAllAssign();
	case PatternMode::ARG1:
		return storage.getAssignByArg1(getArg1());
	}

	auto tokens = TokenSplitter(getArg2()).tokenize();
	TokenIterator start = TokenIterator::cbegin(tokens);
	TokenIterator end = TokenIterator::cend(tokens);
	ExpressionNode expNode = ExpressionParser(start, end).parse();
	rhsRef preOrderRHS = DesignExtractorUtil::getPreOrderTraversal(expNode);
	rhsRef inOrderRHS = DesignExtractorUtil::getInOrderTraversal(expNode);

	switch (mode) {
	case PatternMode::COMPLETE_ARG2:
		return storage.getAssignByCompleteArg2(preOrderRHS, inOrderRHS);
	case PatternMode::SUB_ARG2:
		return storage.getAssignBySubArg2(preOrderRHS, inOrderRHS);
	case PatternMode::ARG1_COMPLETE_ARG2:
		return storage.getAssignByArg1CompleteArg2(getArg1(), preOrderRHS, inOrderRHS);
	case PatternMode::ARG1_SUB_ARG2:
		return  storage.getAssignByArg1SubArg2(getArg1(), preOrderRHS, inOrderRHS);
	default:
		return storage.getAllAssign();
	}
}


