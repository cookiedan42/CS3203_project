#include "PatternExtractor.h"

PatternExtractor::PatternExtractor() {}

vector<shared_ptr<InsertPatternRequest>> PatternExtractor::extractPattern(const unique_ptr<ProgramNode> &AST) {
    auto assignPatterns = AssignPatternExtractor().extractAssignPattern(AST);
    for (const auto& pattern : assignPatterns)
        patternRequests.push_back(make_shared<InsertAssignPatternRequest>(pattern));

    auto ifPatterns = IfPatternExtractor().extractIfPattern(AST);
    for (const auto& pattern : ifPatterns)
        patternRequests.push_back(make_shared<InsertIfPatternRequest>(pattern));

    auto whilePatterns = WhilePatternExtractor().extractWhilePattern(AST);
    for (const auto& pattern : whilePatterns)
        patternRequests.push_back(make_shared<InsertWhilePatternRequest>(pattern));

    return patternRequests;
}