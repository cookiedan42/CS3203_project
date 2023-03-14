#include "ProcedureExtractor.h"

ProcedureExtractor::ProcedureExtractor() {}

vector<Ent::Procedure> ProcedureExtractor::extractProcedures(const unique_ptr<ProgramNode>& AST) {
	for (const auto& procedure : AST->getProcedures()) {
		Ent::Procedure entity{ procedure->getName() };
		procedureEntities.push_back(entity);
	}

	return procedureEntities;
}