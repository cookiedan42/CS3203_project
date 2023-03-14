#pragma once

#include "../../Request/GetRequests/GetPatternRequests/GetPatternRequest.h"
#include "../../Request/InsertRequests/InsertPatternRequests/InsertIfPatternRequest.h"

class PatternManager {

public:
	PatternManager();

	PatternResults get(std::shared_ptr<GetPatternRequest>);
	void insert(std::shared_ptr<InsertPatternRequest> request);

private:
	PatternStorage* patternStorage;
};