#pragma once
#include <memory>
#include <vector>
#include "../../../Request/InsertRequests/InsertEntityRequests/InsertConstantEntityRequest.h"
#include "../../../Request/InsertRequests/InsertEntityRequests/InsertEntityRequest.h"
#include "../../../Request/InsertRequests/InsertEntityRequests/InsertProcedureEntityRequest.h"
#include "../../../Request/InsertRequests/InsertEntityRequests/InsertStatementEntityRequest.h"
#include "../../../Request/InsertRequests/InsertEntityRequests/InsertVariableEntityRequest.h"
#include "../../AST/AST.h"
#include "ConstantExtractor.h"
#include "ProcedureExtractor.h"
#include "StatementExtractor.h"
#include "VariableExtractor.h"

using namespace SP;
using namespace std;

class EntityExtractor {
private:
	vector<shared_ptr<InsertEntityRequest>> entityRequests{};

public:
	EntityExtractor();

	vector<shared_ptr<InsertEntityRequest>> extractEntities(const unique_ptr<ProgramNode> &AST);
};
