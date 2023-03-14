#pragma once
#include <memory>
#include <vector>
#include "../../../Request/InsertRequests/InsertPatternRequests/InsertAssignPatternRequest.h"
#include "../../../Request/InsertRequests/InsertPatternRequests/InsertIfPatternRequest.h"
#include "../../../Request/InsertRequests/InsertPatternRequests/InsertPatternRequest.h"
#include "../../../Request/InsertRequests/InsertPatternRequests/InsertWhilePatternRequest.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"
#include "AssignPatternExtractor.h"
#include "IfPatternExtractor.h"
#include "WhilePatternExtractor.h"

using namespace SP;
using namespace std;

class PatternExtractor {
private:
	vector<shared_ptr<InsertPatternRequest>> patternRequests{};

public:
	PatternExtractor();

	vector<shared_ptr<InsertPatternRequest>> extractPattern(const unique_ptr<ProgramNode> &AST);
};