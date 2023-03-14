#include "ConstantExtractor.h"

void ConstantExtractor::extractStatementListEntity(const vector<StatementNode>& statements) {
	for (const auto& statement : statements) {
		visit([&](auto& stmt) {
			extractConstantEntity(stmt);
			}, statement);
	}
}

void ConstantExtractor::extractConstantEntity(const unique_ptr<AssignStatementNode>& assign) {
	visit([this](auto& expression) {
		extractExpressionEntity(expression);
		}, assign->getRHS());
}

void ConstantExtractor::extractConstantEntity(const unique_ptr<WhileStatementNode>& whiles) {
	extractConditionEntity(whiles->getCondition());

	extractStatementListEntity(whiles->getStatements());
}

void ConstantExtractor::extractConstantEntity(const unique_ptr<IfStatementNode>& ifs) {
	extractConditionEntity(ifs->getCondition());

	extractStatementListEntity(ifs->getThenStatements());
	extractStatementListEntity(ifs->getElseStatements());
}

template<typename T>
void ConstantExtractor::extractConstantEntity(const T& stmt) {
	return;
}

void ConstantExtractor::extractConditionEntity(const unique_ptr<ConditionalExpressionNode>& condition) {
	if (condition->getOp() == SIMPLE::NOT) { // NOT operator
		extractConditionEntity(condition->getCondLHS());
	}
	else if (condition->getOp() == SIMPLE::AND || condition->getOp() == SIMPLE::OR) { // AND or OR operator
		extractConditionEntity(condition->getCondLHS());
		extractConditionEntity(condition->getCondRHS());
	}
	else { // Relational Expression
		visit([this](auto& factor) { // Relational factor LHS
			extractExpressionEntity(factor);
			}, condition->getRelLHS());
		visit([this](auto& factor) { // Relational factor RHS
			extractExpressionEntity(factor);
			}, condition->getRelRHS());
	}
}

void ConstantExtractor::extractExpressionEntity(const unique_ptr<BinaryOperatorNode>& expression) {
	visit([this](auto& expr) {
		extractExpressionEntity(expr);
		}, expression->getLHS());

	visit([this](auto& expr) {
		extractExpressionEntity(expr);
		}, expression->getRHS());
}

void ConstantExtractor::extractExpressionEntity(const unique_ptr<VariableNode>& variable) {
	return;
}
void ConstantExtractor::extractExpressionEntity(const unique_ptr<ConstantNode>& constant) {
	Ent::Constant entity{ constant->getValue() };
	constantEntities.push_back(entity);
}

ConstantExtractor::ConstantExtractor() {}

vector<Ent::Constant> ConstantExtractor::extractConstants(const unique_ptr<ProgramNode>& AST) {
	for (const auto& procedure : AST->getProcedures()) {
		extractStatementListEntity(procedure->getStatements());
	}

	return constantEntities;
}