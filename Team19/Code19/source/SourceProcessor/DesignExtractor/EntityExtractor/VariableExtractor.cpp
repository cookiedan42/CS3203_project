#include "VariableExtractor.h"

void VariableExtractor::extractStatementListVariables(const vector<StatementNode>& statements) {
	for (const auto& statement : statements) {
		visit([&](auto& stmt) {
			extractVariableEntity(stmt);
			}, statement);
	}
}

void VariableExtractor::extractVariableEntity(const unique_ptr<ReadStatementNode>& read) {
	Ent::Variable entity{ read->getVariable()->getName() };
	variableEntities.push_back(entity);
}

void VariableExtractor::extractVariableEntity(const unique_ptr<PrintStatementNode>& print) {
	Ent::Variable entity{ print->getVariable()->getName() };
	variableEntities.push_back(entity);
}

void VariableExtractor::extractVariableEntity(const unique_ptr<AssignStatementNode>& assign) {
	Ent::Variable entity{ assign->getLHS()->getName() };
	variableEntities.push_back(entity);

	vector<Ent::Variable> expressionVariables;
	DesignExtractorUtil::extractExpressionVariables(expressionVariables, assign->getRHS());
	variableEntities.insert(variableEntities.cend(), expressionVariables.cbegin(), expressionVariables.cend());
}

void VariableExtractor::extractVariableEntity(const unique_ptr<WhileStatementNode>& whiles) {
	vector<Ent::Variable> controlVariables;
	DesignExtractorUtil::extractConditionVariables(controlVariables, whiles->getCondition());
	variableEntities.insert(variableEntities.cend(), controlVariables.cbegin(), controlVariables.cend());

	extractStatementListVariables(whiles->getStatements());
}

void VariableExtractor::extractVariableEntity(const unique_ptr<IfStatementNode>& ifs) {
	vector<Ent::Variable> controlVariables;
	DesignExtractorUtil::extractConditionVariables(controlVariables, ifs->getCondition());
	variableEntities.insert(variableEntities.cend(), controlVariables.cbegin(), controlVariables.cend());

	extractStatementListVariables(ifs->getThenStatements());
	extractStatementListVariables(ifs->getElseStatements());
}

void VariableExtractor::extractVariableEntity(const unique_ptr<CallStatementNode>& call) {
	return;
}

VariableExtractor::VariableExtractor() {}

vector<Ent::Variable> VariableExtractor::extractVariables(const unique_ptr<ProgramNode>& AST) {
	for (const auto& procedure : AST->getProcedures()) {
		extractStatementListVariables(procedure->getStatements());
	}

	return variableEntities;
}