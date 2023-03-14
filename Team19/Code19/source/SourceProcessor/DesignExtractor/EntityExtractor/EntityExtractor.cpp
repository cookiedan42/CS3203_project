#include "EntityExtractor.h"

EntityExtractor::EntityExtractor() {}

vector<shared_ptr<InsertEntityRequest>> EntityExtractor::extractEntities(const unique_ptr<ProgramNode> &AST) {
	auto procedureEntities = ProcedureExtractor().extractProcedures(AST);
	for (const auto& procedure : procedureEntities)
		entityRequests.push_back(make_shared<InsertProcedureEntityRequest>(procedure));

	auto statementEntities = StatementExtractor().extractStatements(AST);
	for (const auto& statement : statementEntities)
		entityRequests.push_back(make_shared<InsertStatementEntityRequest>(statement));

	auto variableEntities = VariableExtractor().extractVariables(AST);
	for (const auto& variable : variableEntities)
		entityRequests.push_back(make_shared<InsertVariableEntityRequest>(variable));

	auto constantEntities = ConstantExtractor().extractConstants(AST);
	for (const auto& constant : constantEntities)
		entityRequests.push_back(make_shared<InsertConstantEntityRequest>(constant));

	return move(entityRequests);
}
