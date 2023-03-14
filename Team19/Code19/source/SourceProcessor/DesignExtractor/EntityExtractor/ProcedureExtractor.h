#pragma once
#include <memory>
#include <vector>
#include "../../../Entity/Entity.h"
#include "../../AST/AST.h"

using namespace SP;
using namespace std;

class ProcedureExtractor {
private:
	vector<Ent::Procedure> procedureEntities{};

public:
	ProcedureExtractor();

	vector<Ent::Procedure> extractProcedures(const unique_ptr<ProgramNode>& AST);
};